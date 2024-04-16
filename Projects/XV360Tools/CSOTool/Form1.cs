using System.Runtime.InteropServices;
using XV360Lib;

namespace CSOTool
{
    public partial class Form1 : Form
    {
        CSO cso = new CSO();
        string fileName;
        public Form1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = ".cso Files|*.cso";
            ofd.Title = "Select .cso File";

            if (ofd.ShowDialog() != DialogResult.OK)
                return;

            fileName = ofd.FileName;

            cso.Load(fileName);

            cbChar.Items.Clear();
            for (int i = 0; i < cso.Data.Length; i++)
            {
                cbChar.Items.Add($"{cso.Data[i].Char_ID} - {cso.Data[i].Costume_ID}");
            }
            cbChar.SelectedIndex = -1;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Save(fileName);

            cbChar.Items.Clear();
            for (int i = 0; i < cso.Data.Length; i++)
            {
                cbChar.Items.Add($"{cso.Data[i].Char_ID} - {cso.Data[i].Costume_ID}");
            }
            cbChar.SelectedIndex = -1;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            txtPath1.Text = cso.Data[cbChar.SelectedIndex].Paths[0];
            txtPath2.Text = cso.Data[cbChar.SelectedIndex].Paths[1];
            txtPath3.Text = cso.Data[cbChar.SelectedIndex].Paths[2];
            txtPath4.Text = cso.Data[cbChar.SelectedIndex].Paths[3];
            txtID.Text = cso.Data[cbChar.SelectedIndex].Char_ID.ToString();
            txtCost.Text = cso.Data[cbChar.SelectedIndex].Costume_ID.ToString();
        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Costume_ID = int.Parse(txtCost.Text);
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Char_ID = int.Parse(txtID.Text);
        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Paths[0] = txtPath1.Text;
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Paths[1] = txtPath2.Text;
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Paths[2] = txtPath3.Text;
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            if (fileName == "")
                return;
            cso.Data[cbChar.SelectedIndex].Paths[3] = txtPath4.Text;
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (fileName == "")
                return;


            cso.RemoveCharacter(cso.Data[cbChar.SelectedIndex].Char_ID);

            cbChar.Items.Clear();
            for (int i = 0; i < cso.Data.Length; i++)
            {
                cbChar.Items.Add($"{cso.Data[i].Char_ID} - {cso.Data[i].Costume_ID}");
            }
            cbChar.SelectedIndex = -1;
        }

        private void removeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (fileName == "")
                return;

            cso.AddCharacter(999, 0, new string[4]);

            cbChar.Items.Clear();
            for (int i = 0; i < cso.Data.Length; i++)
            {
                cbChar.Items.Add($"{cso.Data[i].Char_ID} - {cso.Data[i].Costume_ID}");
            }
            cbChar.SelectedIndex = -1;
        }
    }
}
