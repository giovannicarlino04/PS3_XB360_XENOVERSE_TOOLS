namespace CMSTool
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label1 = new System.Windows.Forms.Label();
            this.txtInfo1 = new System.Windows.Forms.TextBox();
            this.txtInfo2 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtInfo3 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtInfo4 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtInfo5 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtInfo6 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtInfo7 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.cbChar = new System.Windows.Forms.ComboBox();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(275, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 84);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Character";
            // 
            // txtInfo1
            // 
            this.txtInfo1.Location = new System.Drawing.Point(117, 81);
            this.txtInfo1.Name = "txtInfo1";
            this.txtInfo1.Size = new System.Drawing.Size(146, 20);
            this.txtInfo1.TabIndex = 2;
            this.txtInfo1.TextChanged += new System.EventHandler(this.txtInfo1_TextChanged);
            // 
            // txtInfo2
            // 
            this.txtInfo2.Location = new System.Drawing.Point(117, 107);
            this.txtInfo2.Name = "txtInfo2";
            this.txtInfo2.Size = new System.Drawing.Size(146, 20);
            this.txtInfo2.TabIndex = 4;
            this.txtInfo2.TextChanged += new System.EventHandler(this.txtInfo2_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 110);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "EAN";
            // 
            // txtInfo3
            // 
            this.txtInfo3.Location = new System.Drawing.Point(117, 133);
            this.txtInfo3.Name = "txtInfo3";
            this.txtInfo3.Size = new System.Drawing.Size(146, 20);
            this.txtInfo3.TabIndex = 6;
            this.txtInfo3.TextChanged += new System.EventHandler(this.txtInfo3_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 136);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(19, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "3?";
            // 
            // txtInfo4
            // 
            this.txtInfo4.Location = new System.Drawing.Point(117, 157);
            this.txtInfo4.Name = "txtInfo4";
            this.txtInfo4.Size = new System.Drawing.Size(146, 20);
            this.txtInfo4.TabIndex = 8;
            this.txtInfo4.TextChanged += new System.EventHandler(this.txtInfo4_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 160);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(85, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Skills Animations";
            // 
            // txtInfo5
            // 
            this.txtInfo5.Location = new System.Drawing.Point(117, 183);
            this.txtInfo5.Name = "txtInfo5";
            this.txtInfo5.Size = new System.Drawing.Size(146, 20);
            this.txtInfo5.TabIndex = 10;
            this.txtInfo5.TextChanged += new System.EventHandler(this.txtInfo5_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(17, 186);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(30, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "BCM";
            // 
            // txtInfo6
            // 
            this.txtInfo6.Location = new System.Drawing.Point(117, 209);
            this.txtInfo6.Name = "txtInfo6";
            this.txtInfo6.Size = new System.Drawing.Size(146, 20);
            this.txtInfo6.TabIndex = 12;
            this.txtInfo6.TextChanged += new System.EventHandler(this.txtInfo6_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(17, 212);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(28, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "BAC";
            // 
            // txtInfo7
            // 
            this.txtInfo7.Location = new System.Drawing.Point(117, 235);
            this.txtInfo7.Name = "txtInfo7";
            this.txtInfo7.Size = new System.Drawing.Size(146, 20);
            this.txtInfo7.TabIndex = 14;
            this.txtInfo7.TextChanged += new System.EventHandler(this.txtInfo7_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 238);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(19, 13);
            this.label7.TabIndex = 13;
            this.label7.Text = "7?";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(21, 35);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 13);
            this.label8.TabIndex = 15;
            this.label8.Text = "Character";
            // 
            // cbChar
            // 
            this.cbChar.FormattingEnabled = true;
            this.cbChar.Location = new System.Drawing.Point(20, 51);
            this.cbChar.Name = "cbChar";
            this.cbChar.Size = new System.Drawing.Size(121, 21);
            this.cbChar.TabIndex = 16;
            this.cbChar.SelectedIndexChanged += new System.EventHandler(this.cbChar_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(275, 271);
            this.Controls.Add(this.cbChar);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.txtInfo7);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.txtInfo6);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.txtInfo5);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txtInfo4);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtInfo3);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtInfo2);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtInfo1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "CMS Tool";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtInfo1;
        private System.Windows.Forms.TextBox txtInfo2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtInfo3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtInfo4;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtInfo5;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtInfo6;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtInfo7;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox cbChar;
    }
}

