using System.Runtime.CompilerServices;
using XV360Lib;

namespace CMSTool
{
    public partial class Form1 : Form
    {
        public string filePath = "";
        CMS cms = new CMS();

        public Form1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open CMS File";
            ofd.Filter = ".cms files|*.cms";

            if (ofd.ShowDialog() == DialogResult.Cancel)
                return;
            filePath = ofd.FileName;

            cms.Load(filePath);

            cbModels.Items.Clear();
            for (int i = 0; i < cms.Data.Length; i++)
            {
                cbModels.Items.Add($"{cms.Data[i].ID} - {cms.Data[i].ShortName}");
            }
            cbModels.SelectedIndex = -1;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Save();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cbModels_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtID.Text = cms.Data[cbModels.SelectedIndex].ID.ToString();
            txtShortName.Text = cms.Data[cbModels.SelectedIndex].ShortName;
            txtPath1.Text = cms.Data[cbModels.SelectedIndex].Paths[0];
            txtPath2.Text = cms.Data[cbModels.SelectedIndex].Paths[1];
            txtPath3.Text = cms.Data[cbModels.SelectedIndex].Paths[2];
            txtPath4.Text = cms.Data[cbModels.SelectedIndex].Paths[3];
            txtPath5.Text = cms.Data[cbModels.SelectedIndex].Paths[4];
            txtPath6.Text = cms.Data[cbModels.SelectedIndex].Paths[5];
            txtPath7.Text = cms.Data[cbModels.SelectedIndex].Paths[6];
        }
        private void txtID_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].ID = int.Parse(txtID.Text);
        }

        private void txtShortName_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].ShortName = txtShortName.Text;
        }

        private void txtPath1_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[0] = txtPath1.Text;
        }

        private void txtPath2_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[1] = txtPath2.Text;
        }

        private void txtPath3_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[2] = txtPath3.Text;
        }

        private void txtPath4_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[3] = txtPath4.Text;
        }

        private void txtPath5_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[4] = txtPath5.Text;

        }

        private void txtPath6_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[5] = txtPath6.Text;
        }

        private void txtPath7_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.Data[cbModels.SelectedIndex].Paths[6] = txtPath7.Text;
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;

            int id = 999;
            string shortName = "ZZZ"; 
            byte[] unknown = new byte[8]; 
            string[] paths =  { "ZZZ", "ZZZ", "ZZZ", "ZZZ", "ZZZ", "ZZZ", "ZZZ" };

            cms.AddModel(id, shortName, unknown, paths);
            cbModels.Items.Clear();
            for (int i = 0; i < cms.Data.Length; i++)
            {
                cbModels.Items.Add($"{cms.Data[i].ID} - {cms.Data[i].ShortName}");
            }
            cbModels.SelectedIndex = -1;
        }

        private void removeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            cms.RemoveModel(cms.Data[cbModels.SelectedIndex].ID);
            cbModels.Items.Clear();
            for (int i = 0; i < cms.Data.Length; i++)
            {
                cbModels.Items.Add($"{cms.Data[i].ID} - {cms.Data[i].ShortName}");
            }
            cbModels.SelectedIndex = -1;
        }
    }
}
