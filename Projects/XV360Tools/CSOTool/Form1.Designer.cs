namespace CSOTool
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
            addToolStripMenuItem = new ToolStripMenuItem();
            removeToolStripMenuItem = new ToolStripMenuItem();
            cbChar = new ComboBox();
            label1 = new Label();
            txtPath1 = new TextBox();
            txtPath2 = new TextBox();
            txtPath3 = new TextBox();
            txtPath4 = new TextBox();
            label2 = new Label();
            txtID = new TextBox();
            txtCost = new TextBox();
            label3 = new Label();
            label4 = new Label();
            menuStrip1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, toolsToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(309, 24);
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
            toolsToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { addToolStripMenuItem, removeToolStripMenuItem });
            toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            toolsToolStripMenuItem.Size = new Size(46, 20);
            toolsToolStripMenuItem.Text = "Tools";
            // 
            // addToolStripMenuItem
            // 
            addToolStripMenuItem.Name = "addToolStripMenuItem";
            addToolStripMenuItem.Size = new Size(180, 22);
            addToolStripMenuItem.Text = "Add";
            addToolStripMenuItem.Click += addToolStripMenuItem_Click;
            // 
            // removeToolStripMenuItem
            // 
            removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            removeToolStripMenuItem.Size = new Size(180, 22);
            removeToolStripMenuItem.Text = "Remove";
            removeToolStripMenuItem.Click += removeToolStripMenuItem_Click;
            // 
            // cbChar
            // 
            cbChar.FormattingEnabled = true;
            cbChar.Location = new Point(30, 66);
            cbChar.Name = "cbChar";
            cbChar.Size = new Size(121, 23);
            cbChar.TabIndex = 1;
            cbChar.SelectedIndexChanged += comboBox1_SelectedIndexChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(33, 48);
            label1.Name = "label1";
            label1.Size = new Size(114, 15);
            label1.TabIndex = 2;
            label1.Text = "Character/Costume:";
            // 
            // txtPath1
            // 
            txtPath1.Location = new Point(41, 110);
            txtPath1.Name = "txtPath1";
            txtPath1.Size = new Size(227, 23);
            txtPath1.TabIndex = 3;
            txtPath1.TextChanged += textBox1_TextChanged;
            // 
            // txtPath2
            // 
            txtPath2.Location = new Point(41, 139);
            txtPath2.Name = "txtPath2";
            txtPath2.Size = new Size(227, 23);
            txtPath2.TabIndex = 3;
            txtPath2.TextChanged += textBox2_TextChanged;
            // 
            // txtPath3
            // 
            txtPath3.Location = new Point(41, 168);
            txtPath3.Name = "txtPath3";
            txtPath3.Size = new Size(227, 23);
            txtPath3.TabIndex = 3;
            txtPath3.TextChanged += textBox3_TextChanged;
            // 
            // txtPath4
            // 
            txtPath4.Location = new Point(41, 197);
            txtPath4.Name = "txtPath4";
            txtPath4.Size = new Size(227, 23);
            txtPath4.TabIndex = 3;
            txtPath4.TextChanged += textBox4_TextChanged;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(134, 92);
            label2.Name = "label2";
            label2.Size = new Size(39, 15);
            label2.TabIndex = 4;
            label2.Text = "Paths:";
            // 
            // txtID
            // 
            txtID.Location = new Point(157, 66);
            txtID.Name = "txtID";
            txtID.Size = new Size(52, 23);
            txtID.TabIndex = 5;
            txtID.TextChanged += textBox5_TextChanged;
            // 
            // txtCost
            // 
            txtCost.Location = new Point(215, 66);
            txtCost.Name = "txtCost";
            txtCost.Size = new Size(52, 23);
            txtCost.TabIndex = 6;
            txtCost.TextChanged += textBox6_TextChanged;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(172, 48);
            label3.Name = "label3";
            label3.Size = new Size(21, 15);
            label3.TabIndex = 7;
            label3.Text = "ID:";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(211, 48);
            label4.Name = "label4";
            label4.Size = new Size(58, 15);
            label4.TabIndex = 7;
            label4.Text = "Costume:";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(309, 251);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(txtCost);
            Controls.Add(txtID);
            Controls.Add(label2);
            Controls.Add(txtPath4);
            Controls.Add(txtPath3);
            Controls.Add(txtPath2);
            Controls.Add(txtPath1);
            Controls.Add(label1);
            Controls.Add(cbChar);
            Controls.Add(menuStrip1);
            FormBorderStyle = FormBorderStyle.FixedToolWindow;
            MainMenuStrip = menuStrip1;
            Name = "Form1";
            Text = "Form1";
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
        private ToolStripMenuItem toolsToolStripMenuItem;
        private ToolStripMenuItem addToolStripMenuItem;
        private ToolStripMenuItem removeToolStripMenuItem;
        private ComboBox cbChar;
        private Label label1;
        private TextBox txtPath1;
        private TextBox txtPath2;
        private TextBox txtPath3;
        private TextBox txtPath4;
        private Label label2;
        private TextBox txtID;
        private TextBox txtCost;
        private Label label3;
        private Label label4;
    }
}
