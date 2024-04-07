using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace CPKTools
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            cbCode.SelectedIndex = 0;
            cbMode.SelectedIndex = 0;
        }

        //打开CPK
        private void btnBECpkFile_Click(object sender, EventArgs e)
        {
            if (openCPKDialog.ShowDialog() == DialogResult.OK)
            {
                tbECpkFile.Text = openCPKDialog.FileName;
            }
        }

        //保存解压
        private void btnEPath_Click(object sender, EventArgs e)
        {
            if (saveExtractFolder.ShowDialog() == DialogResult.OK)
            {
                tbEPath.Text = saveExtractFolder.SelectedPath;
            }
        }

        //点击解压
        private void btnExtract_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbECpkFile.Text))
            {
                MessageBox.Show("Please select the file to Decompress", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (string.IsNullOrEmpty(tbEPath.Text))
            {
                MessageBox.Show("Please select where to extract to", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            using (Process proc = new Process())
            {
                proc.StartInfo.FileName = "cmd";
                proc.StartInfo.UseShellExecute = false;
                proc.StartInfo.RedirectStandardInput = true;
                proc.StartInfo.RedirectStandardError = true;
                proc.Start();
                string cmd = string.Format("quickbms.exe cpk.bms \"{0}\" \"{1}\"", tbECpkFile.Text, tbEPath.Text);
                proc.StandardInput.WriteLine(cmd);
                proc.StandardInput.WriteLine("\r\nexit");

                proc.WaitForExit();
            }
            MessageBox.Show("Extraction Complete", "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        //打开压缩路径
        private void btnPackPath_Click(object sender, EventArgs e)
        {
            if (saveExtractFolder.ShowDialog() == DialogResult.OK)
            {
                tbPackPath.Text = saveExtractFolder.SelectedPath;
            }
        }

        //保存压缩
        private void btnSaveCPK_Click(object sender, EventArgs e)
        {
            if (saveCPKDialog.ShowDialog() == DialogResult.OK)
            {
                tbSaveCPK.Text = saveCPKDialog.FileName;
            }
        }

        private void btnCompress_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(tbPackPath.Text))
            {
                MessageBox.Show("Please select the folder you want to compress", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (string.IsNullOrEmpty(tbSaveCPK.Text))
            {
                MessageBox.Show("Please select the file to save", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            using (Process proc = new Process())
            {
                proc.StartInfo.FileName = "cpkmakec.exe";
                string arg = string.Format("\"{0}\" \"{1}\" -align={2} -code={3} -mode={4}", tbPackPath.Text, tbSaveCPK.Text, numDataAlign.Value, cbCode.SelectedItem, cbMode.SelectedItem);
                if (cbForceCom.Checked)
                    arg += " -forcecompress";
                if (cbDMask.Checked)
                    arg += " -mask";
                proc.StartInfo.Arguments = arg;
                proc.Start();
                proc.WaitForExit();
            }
            MessageBox.Show("Compression Complete", "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
