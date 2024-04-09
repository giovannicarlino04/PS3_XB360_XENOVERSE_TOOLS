namespace CMSTool
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
            txtPath1 = new TextBox();
            cbModels = new ComboBox();
            txtPath2 = new TextBox();
            txtPath3 = new TextBox();
            txtPath4 = new TextBox();
            txtPath5 = new TextBox();
            txtPath6 = new TextBox();
            txtPath7 = new TextBox();
            label1 = new Label();
            label2 = new Label();
            txtID = new TextBox();
            txtShortName = new TextBox();
            label3 = new Label();
            menuStrip1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, toolsToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(193, 24);
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
            addToolStripMenuItem.Size = new Size(117, 22);
            addToolStripMenuItem.Text = "Add";
            addToolStripMenuItem.Click += addToolStripMenuItem_Click;
            // 
            // removeToolStripMenuItem
            // 
            removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            removeToolStripMenuItem.Size = new Size(117, 22);
            removeToolStripMenuItem.Text = "Remove";
            removeToolStripMenuItem.Click += removeToolStripMenuItem_Click;
            // 
            // txtPath1
            // 
            txtPath1.Location = new Point(74, 90);
            txtPath1.Name = "txtPath1";
            txtPath1.Size = new Size(100, 23);
            txtPath1.TabIndex = 1;
            txtPath1.TextChanged += txtPath1_TextChanged;
            // 
            // cbModels
            // 
            cbModels.FormattingEnabled = true;
            cbModels.Location = new Point(14, 46);
            cbModels.Name = "cbModels";
            cbModels.Size = new Size(162, 23);
            cbModels.TabIndex = 2;
            cbModels.SelectedIndexChanged += cbModels_SelectedIndexChanged;
            // 
            // txtPath2
            // 
            txtPath2.Location = new Point(74, 119);
            txtPath2.Name = "txtPath2";
            txtPath2.Size = new Size(100, 23);
            txtPath2.TabIndex = 1;
            txtPath2.TextChanged += txtPath2_TextChanged;
            // 
            // txtPath3
            // 
            txtPath3.Location = new Point(74, 148);
            txtPath3.Name = "txtPath3";
            txtPath3.Size = new Size(100, 23);
            txtPath3.TabIndex = 1;
            txtPath3.TextChanged += txtPath3_TextChanged;
            // 
            // txtPath4
            // 
            txtPath4.Location = new Point(74, 177);
            txtPath4.Name = "txtPath4";
            txtPath4.Size = new Size(100, 23);
            txtPath4.TabIndex = 1;
            txtPath4.TextChanged += txtPath4_TextChanged;
            // 
            // txtPath5
            // 
            txtPath5.Location = new Point(74, 206);
            txtPath5.Name = "txtPath5";
            txtPath5.Size = new Size(100, 23);
            txtPath5.TabIndex = 1;
            txtPath5.TextChanged += txtPath5_TextChanged;
            // 
            // txtPath6
            // 
            txtPath6.Location = new Point(74, 235);
            txtPath6.Name = "txtPath6";
            txtPath6.Size = new Size(100, 23);
            txtPath6.TabIndex = 1;
            txtPath6.TextChanged += txtPath6_TextChanged;
            // 
            // txtPath7
            // 
            txtPath7.Location = new Point(74, 264);
            txtPath7.Name = "txtPath7";
            txtPath7.Size = new Size(100, 23);
            txtPath7.TabIndex = 1;
            txtPath7.TextChanged += txtPath7_TextChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(14, 28);
            label1.Name = "label1";
            label1.Size = new Size(58, 15);
            label1.TabIndex = 3;
            label1.Text = "Model ID:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(103, 72);
            label2.Name = "label2";
            label2.Size = new Size(39, 15);
            label2.TabIndex = 3;
            label2.Text = "Paths:";
            // 
            // txtID
            // 
            txtID.Location = new Point(14, 90);
            txtID.MaxLength = 3;
            txtID.Name = "txtID";
            txtID.Size = new Size(54, 23);
            txtID.TabIndex = 4;
            txtID.TextChanged += txtID_TextChanged;
            // 
            // txtShortName
            // 
            txtShortName.Location = new Point(14, 119);
            txtShortName.MaxLength = 3;
            txtShortName.Name = "txtShortName";
            txtShortName.Size = new Size(54, 23);
            txtShortName.TabIndex = 4;
            txtShortName.TextChanged += txtShortName_TextChanged;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(15, 72);
            label3.Name = "label3";
            label3.Size = new Size(55, 15);
            label3.TabIndex = 3;
            label3.Text = "ID/Name";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(193, 296);
            Controls.Add(txtShortName);
            Controls.Add(txtID);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(cbModels);
            Controls.Add(txtPath7);
            Controls.Add(txtPath6);
            Controls.Add(txtPath5);
            Controls.Add(txtPath4);
            Controls.Add(txtPath3);
            Controls.Add(txtPath2);
            Controls.Add(txtPath1);
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
        private TextBox txtPath1;
        private ComboBox cbModels;
        private TextBox txtPath2;
        private TextBox txtPath3;
        private TextBox txtPath4;
        private TextBox txtPath5;
        private TextBox txtPath6;
        private TextBox txtPath7;
        private Label label1;
        private Label label2;
        private TextBox txtID;
        private TextBox txtShortName;
        private Label label3;
    }
}
