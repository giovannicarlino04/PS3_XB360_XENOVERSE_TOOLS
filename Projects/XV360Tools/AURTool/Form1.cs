using XV360Lib;

namespace AURTool
{
    public partial class Form1 : Form
    {
        public string filePath = "";
        AUR aur = new AUR();
        public Form1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open AUR File";
            ofd.Filter = ".aur files|*.aur";

            if (ofd.ShowDialog() == DialogResult.Cancel)
                return;
            filePath = ofd.FileName;

            aur.load(filePath);


            cbChar.Items.Clear();
            for (int i = 0; i < aur.Chars.Length; i++)
            {
                cbChar.Items.Add($"Character {aur.Chars[i].Name} - Costume {aur.Chars[i].Costume}");
            }
            cbChar.SelectedIndex = -1;
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;

            aur.save(filePath);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void addCharacterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;

            aur.AddCharacter(999, 999, 0, false);

            cbChar.Items.Clear();
            for (int i = 0; i < aur.Chars.Length; i++)
            {
                cbChar.Items.Add($"Character {aur.Chars[i].Name} - Costume {aur.Chars[i].Costume}");
            }
            cbChar.SelectedIndex = -1;
        }

        private void removeCharacterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filePath == "")
                return;

            aur.RemoveCharacter(aur.Chars[cbChar.SelectedIndex].Name);

            cbChar.Items.Clear();
            for (int i = 0; i < aur.Chars.Length; i++)
            {
                cbChar.Items.Add($"Character {aur.Chars[i].Name} - Costume {aur.Chars[i].Costume}");
            }
            cbChar.SelectedIndex = -1;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            txtCharID.Text = aur.Chars[cbChar.SelectedIndex].Name.ToString();
            txtCostID.Text = aur.Chars[cbChar.SelectedIndex].Costume.ToString();
            txtAuraID.Text = aur.Chars[cbChar.SelectedIndex].ID.ToString();
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            aur.Chars[cbChar.SelectedIndex].Name = int.Parse(txtCharID.Text);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            aur.Chars[cbChar.SelectedIndex].ID = int.Parse(txtAuraID.Text);
        }

        private void txtCostID_TextChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            aur.Chars[cbChar.SelectedIndex].Costume = int.Parse(txtCostID.Text);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (filePath == "")
                return;
            aur.Chars[cbChar.SelectedIndex].inf = checkBox1.Checked;
        }
    }
}
