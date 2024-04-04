using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CMSTool
{
    public partial class Form1 : Form
    {
        CMS file;
        string FileName;
        bool Lock = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            file = new CMS();

            OpenFileDialog browseFile = new OpenFileDialog();
            browseFile.Filter = "Xenoverse Custom Model Specification File (*.cms)|*.cms";
            browseFile.Title = "Browse for Xenoverse Custom Model Specification File";
            if (browseFile.ShowDialog() == DialogResult.Cancel)
                return;

            file.Load(browseFile.FileName);

            cbChar.Items.Clear();
            for (int i = 0; i < file.Data.Length; i++)
                cbChar.Items.Add(file.Data[i].ShortName);

        }

        private void cbChar_SelectedIndexChanged(object sender, EventArgs e)
        {
            Lock = true;
            txtInfo1.Text = file.Data[cbChar.SelectedIndex].Paths[0];
            txtInfo2.Text = file.Data[cbChar.SelectedIndex].Paths[1];
            txtInfo3.Text = file.Data[cbChar.SelectedIndex].Paths[2];
            txtInfo4.Text = file.Data[cbChar.SelectedIndex].Paths[3];
            txtInfo5.Text = file.Data[cbChar.SelectedIndex].Paths[4];
            txtInfo6.Text = file.Data[cbChar.SelectedIndex].Paths[5];
            txtInfo7.Text = file.Data[cbChar.SelectedIndex].Paths[6];
            Lock = false;
        }

        private void txtInfo1_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
            file.Data[cbChar.SelectedIndex].Paths[0] = txtInfo1.Text;
        }

        private void txtInfo2_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[1] = txtInfo2.Text;
        }

        private void txtInfo3_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[2] = txtInfo3.Text;
        }

        private void txtInfo4_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[3] = txtInfo4.Text;
        }

        private void txtInfo5_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[4] = txtInfo5.Text;
        }

        private void txtInfo6_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[5] = txtInfo6.Text;
        }

        private void txtInfo7_TextChanged(object sender, EventArgs e)
        {
            if (!Lock)
                file.Data[cbChar.SelectedIndex].Paths[6] = txtInfo7.Text;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            file.Save();
        }
    }
}
