#include "Stream.h"

#ifndef NO_CRYPTO
#include "crypto/sha1.h"
#endif

#include "debug.h"

#define BUFFER_SIZE	(16*1024*1024)

bool Stream::Copy(Stream *other, size_t size)
{
    uint8_t *copy_buf;
    size_t remaining;

    if (size <BUFFER_SIZE)
    {
        copy_buf = new uint8_t[size];
    }
    else
    {
        copy_buf = new uint8_t[BUFFER_SIZE];
    }

    remaining = size;

    while (remaining > 0)
    {
        size_t r = (remaining > BUFFER_SIZE) ? BUFFER_SIZE: remaining;

        if (!other->Read(copy_buf, r))
        {
            delete[] copy_buf;
            return false;
        }

        if (!Write(copy_buf, r))
        {
            delete[] copy_buf;
            return false;
        }

        remaining -= r;
    }

    delete[] copy_buf;
    return true;
}

#ifdef NO_CRYPTO

bool Stream::CopyEx(Stream *, size_t, Hash, uint8_t *, Cipher, const uint8_t *, int, Cipher, const uint8_t *, int)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
    return false;
}

#else

bool Stream::CopyEx(Stream *other, size_t size, Hash hash_mode, uint8_t *hash, Cipher decrypt_mode, const uint8_t *decrypt_key, int decrypt_key_size, Cipher encrypt_mode, const uint8_t *encrypt_key, int encrypt_key_size)
{
    static const size_t block_size = 16;
    uint8_t *copy_buf;
    size_t remaining;
    SHA1_CTX ctx;
    bool decrypt, encrypt, do_hash;

    assert((BUFFER_SIZE % block_size) == 0);

    do_hash = (hash_mode == Hash::SHA1);
    decrypt = (decrypt_mode == Cipher::AES_PLAIN);
    encrypt = (encrypt_mode == Cipher::AES_PLAIN);

    if (size < BUFFER_SIZE)
    {
        size_t buf_size = size;

        if (decrypt || encrypt)
        {
            if ((buf_size % block_size) != 0)
            {
                buf_size += (block_size - (buf_size % block_size));
            }
        }

        copy_buf = new uint8_t[buf_size];
    }
    else
    {
        copy_buf = new uint8_t[BUFFER_SIZE];
    }   

    if (do_hash)
        SHA1_Init(&ctx);

    remaining = size;

    while (remaining > 0)
    {
        size_t r = (remaining > BUFFER_SIZE) ? BUFFER_SIZE: remaining;
        size_t read_size = r;
        size_t write_size = r;
        bool eof = false;

        if (decrypt)
        {
            if ((read_size % block_size) != 0)
            {
                read_size += (block_size - (r % block_size));
                eof = true;
            }
        }

        if (!other->Read(copy_buf, read_size))
        {
            delete[] copy_buf;
            return false;
        }

        if (do_hash)
            SHA1_Update(&ctx, copy_buf, (uint32_t)r);

        if (decrypt)
        {
            Utils::PlainAESDecrypt(copy_buf, read_size, decrypt_key, decrypt_key_size);
        }

        if (encrypt)
        {
            if ((write_size % block_size) != 0)
            {
                size_t new_write_size = write_size + (block_size - (r % block_size));

                memset(copy_buf+write_size, 0, new_write_size-write_size);
                write_size = new_write_size;
            }

            Utils::PlainAESEncrypt(copy_buf, write_size, encrypt_key, encrypt_key_size);
        }

        if (!Write(copy_buf, write_size))
        {
            delete[] copy_buf;
            return false;
        }

        if (eof)
            break;

        remaining -= r;
    }

    if (do_hash)
        SHA1_Final(&ctx, hash);

    delete[] copy_buf;
    return true;
}

#endif // NO_CRYPTO

bool Stream::Align(unsigned int alignment)
{
    unsigned int write_size = 0;
    uint64_t file_pos = Tell();

    if ((file_pos % alignment) != 0)
        write_size = (alignment - (file_pos % alignment));

    for (unsigned int i = 0; i < write_size; i++)
    {
        if (!Write8(0))
            return false;
    }

    return true;
}

