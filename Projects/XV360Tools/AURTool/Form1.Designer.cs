namespace AURTool
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem = new ToolStripMenuItem();
            openToolStripMenuItem = new ToolStripMenuItem();
            saveToolStripMenuItem = new ToolStripMenuItem();
            toolStripSeparator1 = new ToolStripSeparator();
            exitToolStripMenuItem = new ToolStripMenuItem();
            toolsToolStripMenuItem = new ToolStripMenuItem();
            addCharacterToolStripMenuItem = new ToolStripMenuItem();
            removeCharacterToolStripMenuItem = new ToolStripMenuItem();
            cbChar = new ComboBox();
            txtAuraID = new TextBox();
            label1 = new Label();
            label2 = new Label();
            txtCharID = new TextBox();
            label3 = new Label();
            label4 = new Label();
            txtCostID = new TextBox();
            checkBox1 = new CheckBox();
            menuStrip1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, toolsToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(588, 24);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { openToolStripMenuItem, saveToolStripMenuItem, toolStripSeparator1, exitToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new Size(37, 20);
            fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            openToolStripMenuItem.Name = "openToolStripMenuItem";
            openToolStripMenuItem.Size = new Size(103, 22);
            openToolStripMenuItem.Text = "Open";
            openToolStripMenuItem.Click += openToolStripMenuItem_Click;
            // 
            // saveToolStripMenuItem
            // 
            saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            saveToolStripMenuItem.Size = new Size(103, 22);
            saveToolStripMenuItem.Text = "Save";
            saveToolStripMenuItem.Click += saveToolStripMenuItem_Click;
            // 
            // toolStripSeparator1
            // 
            toolStripSeparator1.Name = "toolStripSeparator1";
            toolStripSeparator1.Size = new Size(100, 6);
            // 
            // exitToolStripMenuItem
            // 
            exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            exitToolStripMenuItem.Size = new Size(103, 22);
            exitToolStripMenuItem.Text = "Exit";
            exitToolStripMenuItem.Click += exitToolStripMenuItem_Click;
            // 
            // toolsToolStripMenuItem
            // 
            toolsToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { addCharacterToolStripMenuItem, removeCharacterToolStripMenuItem });
            toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            toolsToolStripMenuItem.Size = new Size(46, 20);
            toolsToolStripMenuItem.Text = "Tools";
            // 
            // addCharacterToolStripMenuItem
            // 
            addCharacterToolStripMenuItem.Name = "addCharacterToolStripMenuItem";
            addCharacterToolStripMenuItem.Size = new Size(180, 22);
            addCharacterToolStripMenuItem.Text = "Add Character";
            addCharacterToolStripMenuItem.Click += addCharacterToolStripMenuItem_Click;
            // 
            // removeCharacterToolStripMenuItem
            // 
            removeCharacterToolStripMenuItem.Name = "removeCharacterToolStripMenuItem";
            removeCharacterToolStripMenuItem.Size = new Size(180, 22);
            removeCharacterToolStripMenuItem.Text = "Remove Character";
            removeCharacterToolStripMenuItem.Click += removeCharacterToolStripMenuItem_Click;
            // 
            // cbChar
            // 
            cbChar.FormattingEnabled = true;
            cbChar.Location = new Point(12, 54);
            cbChar.Name = "cbChar";
            cbChar.Size = new Size(205, 23);
            cbChar.TabIndex = 1;
            cbChar.SelectedIndexChanged += comboBox1_SelectedIndexChanged;
            // 
            // txtAuraID
            // 
            txtAuraID.Location = new Point(435, 54);
            txtAuraID.Name = "txtAuraID";
            txtAuraID.Size = new Size(141, 23);
            txtAuraID.TabIndex = 2;
            txtAuraID.TextChanged += textBox1_TextChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(11, 35);
            label1.Name = "label1";
            label1.Size = new Size(61, 15);
            label1.TabIndex = 3;
            label1.Text = "Character:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(435, 36);
            label2.Name = "label2";
            label2.Size = new Size(49, 15);
            label2.TabIndex = 3;
            label2.Text = "Aura ID:";
            // 
            // txtCharID
            // 
            txtCharID.Location = new Point(223, 54);
            txtCharID.Name = "txtCharID";
            txtCharID.Size = new Size(100, 23);
            txtCharID.TabIndex = 2;
            txtCharID.TextChanged += textBox2_TextChanged;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(223, 34);
            label3.Name = "label3";
            label3.Size = new Size(75, 15);
            label3.TabIndex = 3;
            label3.Text = "Character ID:";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(329, 34);
            label4.Name = "label4";
            label4.Size = new Size(72, 15);
            label4.TabIndex = 5;
            label4.Text = "Costume ID:";
            // 
            // txtCostID
            // 
            txtCostID.Location = new Point(329, 54);
            txtCostID.Name = "txtCostID";
            txtCostID.Size = new Size(100, 23);
            txtCostID.TabIndex = 4;
            txtCostID.TextChanged += txtCostID_TextChanged;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(475, 83);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(53, 19);
            checkBox1.TabIndex = 6;
            checkBox1.Text = "Glare";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = SystemColors.Control;
            ClientSize = new Size(588, 118);
            Controls.Add(checkBox1);
            Controls.Add(label4);
            Controls.Add(txtCostID);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(txtCharID);
            Controls.Add(txtAuraID);
            Controls.Add(cbChar);
            Controls.Add(menuStrip1);
            FormBorderStyle = FormBorderStyle.FixedToolWindow;
            MainMenuStrip = menuStrip1;
            Name = "Form1";
            Text = "AURTool";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem openToolStripMenuItem;
        private ToolStripMenuItem saveToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem exitToolStripMenuItem;
        private ComboBox cbChar;
        private TextBox txtAuraID;
        private Label label1;
        private Label label2;
        private ToolStripMenuItem toolsToolStripMenuItem;
        private ToolStripMenuItem addCharacterToolStripMenuItem;
        private ToolStripMenuItem removeCharacterToolStripMenuItem;
        private TextBox txtCharID;
        private Label label3;
        private Label label4;
        private TextBox txtCostID;
        private CheckBox checkBox1;
    }
}
