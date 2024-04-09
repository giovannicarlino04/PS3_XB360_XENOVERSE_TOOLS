using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace CSSTool
{
    public partial class Form1 : Form
    {
        private byte[] originalData; // Store the original data read from the file
        private string filePath;
        public Form1()
        {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open CHARASELE.iggy file";
            ofd.Filter = "CHARASELE.iggy | CHARASELE.iggy";
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    filePath = ofd.FileName;

                    // Read the file into a byte array
                    originalData = File.ReadAllBytes(ofd.FileName);

                    // Define the offsets
                    int charalistStartOffset = 0x1A139;
                    int charalistEndOffset = 0x1A215;

                    // Extract the characters from the charalist
                    byte[] charalistBytes = new byte[charalistEndOffset - charalistStartOffset];
                    Array.Copy(originalData, charalistStartOffset, charalistBytes, 0, charalistBytes.Length);
                    string charalist = BitConverter.ToString(charalistBytes).Replace("-", "");
                    string[] charalistarr = charalist.Replace("03", "\n").Replace("06", "\n").Split("\n");

                    listBox1.Items.Clear();
                    foreach (string character in charalistarr)
                    {
                        if (character != "\n")
                            listBox1.Items.Add(FromHexString(character));
                    }
                    listBox1.Items.RemoveAt(0);

                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error reading file: " + ex.Message);
                }
            }
        }

        public static string FromHexString(string hexString)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < hexString.Length; i += 2)
            {
                string hexChar = hexString.Substring(i, 2);
                if (byte.TryParse(hexChar, System.Globalization.NumberStyles.HexNumber, null, out byte byteValue))
                {
                    sb.Append((char)byteValue);
                }
                else
                {
                    // Handle invalid hex character
                    MessageBox.Show($"Invalid hexadecimal character: {hexChar}");
                    // You can choose to return null or an empty string here, depending on your requirements
                    return string.Empty;
                }
            }
            return sb.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MoveItem(-1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MoveItem(1);
        }

        private void MoveItem(int direction)
        {
            // Checking selected item
            if (listBox1.SelectedItem == null || listBox1.SelectedIndex < 0)
                return; // No item selected - nothing to move

            // Calculate new index using direction parameter
            int newIndex = listBox1.SelectedIndex + direction;

            // Checking bounds of the list
            if (newIndex < 0 || newIndex >= listBox1.Items.Count)
                return; // Index out of bounds - cannot move further

            // Swap items
            object selectedItem = listBox1.SelectedItem;
            listBox1.Items.Remove(selectedItem);
            listBox1.Items.Insert(newIndex, selectedItem);
            listBox1.SetSelected(newIndex, true);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveChanges();
        }
        private void SaveChanges()
        {
            try
            {
                // Get the updated character list from listBox1 and convert to hexadecimal
                StringBuilder updatedCharalist = new StringBuilder();
                if (listBox1.Items.Count > 0)
                {
                    updatedCharalist.Append("03"); // Add 0x03 at the beginning only if there are characters
                    for (int i = 0; i < listBox1.Items.Count; i++)
                    {
                        updatedCharalist.Append(ToHexString(listBox1.Items[i].ToString()));
                        if (i < listBox1.Items.Count - 1) // Add 0x03 between characters, but not after the last character
                            updatedCharalist.Append("03");
                    }
                }
                updatedCharalist.Append("06"); // Add 0x06 at the end

                string updatedCharalistString = updatedCharalist.ToString();

                // Update the original data with the modified character list
                int charalistStartOffset = 0x1A139;
                int charalistEndOffset = 0x1A215;
                byte[] updatedCharalistBytes = new byte[updatedCharalistString.Length / 2];
                for (int i = 0; i < updatedCharalistString.Length; i += 2)
                {
                    updatedCharalistBytes[i / 2] = Convert.ToByte(updatedCharalistString.Substring(i, 2), 16);
                }
                Array.Copy(updatedCharalistBytes, 0, originalData, charalistStartOffset, updatedCharalistBytes.Length);

                // Write the modified data back to the iggy file
                File.WriteAllBytes(filePath, originalData);

                MessageBox.Show("Changes saved successfully.");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error saving changes: " + ex.Message);
            }
        }


    public static string ToHexString(string asciiString)
        {
            StringBuilder hex = new StringBuilder();
            foreach (char c in asciiString)
            {
                hex.Append(((int)c).ToString("X2"));
            }
            return hex.ToString();
        }
    }
}
