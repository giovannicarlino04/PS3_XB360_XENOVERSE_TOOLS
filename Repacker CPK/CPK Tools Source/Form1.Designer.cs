namespace CPKTools
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnExtract = new System.Windows.Forms.Button();
            this.tbEPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnEPath = new System.Windows.Forms.Button();
            this.btnBECpkFile = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tbECpkFile = new System.Windows.Forms.TextBox();
            this.openCPKDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveExtractFolder = new System.Windows.Forms.FolderBrowserDialog();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnCompress = new System.Windows.Forms.Button();
            this.tbSaveCPK = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cbDMask = new System.Windows.Forms.CheckBox();
            this.cbForceCom = new System.Windows.Forms.CheckBox();
            this.cbMode = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cbCode = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.numDataAlign = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.tbPackPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnSaveCPK = new System.Windows.Forms.Button();
            this.btnPackPath = new System.Windows.Forms.Button();
            this.saveCPKDialog = new System.Windows.Forms.SaveFileDialog();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numDataAlign)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnExtract);
            this.groupBox1.Controls.Add(this.tbEPath);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.btnEPath);
            this.groupBox1.Controls.Add(this.btnBECpkFile);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.tbECpkFile);
            this.groupBox1.Location = new System.Drawing.Point(13, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(394, 127);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Unpack CPK";
            // 
            // btnExtract
            // 
            this.btnExtract.Location = new System.Drawing.Point(160, 92);
            this.btnExtract.Name = "btnExtract";
            this.btnExtract.Size = new System.Drawing.Size(75, 23);
            this.btnExtract.TabIndex = 5;
            this.btnExtract.Text = "Decompress";
            this.btnExtract.UseVisualStyleBackColor = true;
            this.btnExtract.Click += new System.EventHandler(this.btnExtract_Click);
            // 
            // tbEPath
            // 
            this.tbEPath.Location = new System.Drawing.Point(79, 55);
            this.tbEPath.Name = "tbEPath";
            this.tbEPath.ReadOnly = true;
            this.tbEPath.Size = new System.Drawing.Size(236, 21);
            this.tbEPath.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Save Path";
            // 
            // btnEPath
            // 
            this.btnEPath.Location = new System.Drawing.Point(321, 53);
            this.btnEPath.Name = "btnEPath";
            this.btnEPath.Size = new System.Drawing.Size(53, 23);
            this.btnEPath.TabIndex = 2;
            this.btnEPath.Text = "Browse";
            this.btnEPath.UseVisualStyleBackColor = true;
            this.btnEPath.Click += new System.EventHandler(this.btnEPath_Click);
            // 
            // btnBECpkFile
            // 
            this.btnBECpkFile.Location = new System.Drawing.Point(321, 18);
            this.btnBECpkFile.Name = "btnBECpkFile";
            this.btnBECpkFile.Size = new System.Drawing.Size(53, 23);
            this.btnBECpkFile.TabIndex = 2;
            this.btnBECpkFile.Text = "Browse";
            this.btnBECpkFile.UseVisualStyleBackColor = true;
            this.btnBECpkFile.Click += new System.EventHandler(this.btnBECpkFile_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(26, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "CPK File";
            // 
            // tbECpkFile
            // 
            this.tbECpkFile.Location = new System.Drawing.Point(79, 20);
            this.tbECpkFile.Name = "tbECpkFile";
            this.tbECpkFile.ReadOnly = true;
            this.tbECpkFile.Size = new System.Drawing.Size(236, 21);
            this.tbECpkFile.TabIndex = 0;
            // 
            // openCPKDialog
            // 
            this.openCPKDialog.Filter = "CPK File|*.cpk";
            this.openCPKDialog.Title = "Please choose the CPK to extract";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnCompress);
            this.groupBox2.Controls.Add(this.tbSaveCPK);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.cbDMask);
            this.groupBox2.Controls.Add(this.cbForceCom);
            this.groupBox2.Controls.Add(this.cbMode);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.cbCode);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.numDataAlign);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.tbPackPath);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.btnSaveCPK);
            this.groupBox2.Controls.Add(this.btnPackPath);
            this.groupBox2.Location = new System.Drawing.Point(13, 146);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(394, 209);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Build CPK";
            // 
            // btnCompress
            // 
            this.btnCompress.Location = new System.Drawing.Point(160, 178);
            this.btnCompress.Name = "btnCompress";
            this.btnCompress.Size = new System.Drawing.Size(75, 23);
            this.btnCompress.TabIndex = 12;
            this.btnCompress.Text = "Build";
            this.btnCompress.UseVisualStyleBackColor = true;
            this.btnCompress.Click += new System.EventHandler(this.btnCompress_Click);
            // 
            // tbSaveCPK
            // 
            this.tbSaveCPK.Location = new System.Drawing.Point(79, 141);
            this.tbSaveCPK.Name = "tbSaveCPK";
            this.tbSaveCPK.ReadOnly = true;
            this.tbSaveCPK.Size = new System.Drawing.Size(236, 21);
            this.tbSaveCPK.TabIndex = 11;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(20, 144);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 10;
            this.label7.Text = "Save Path";
            // 
            // cbDMask
            // 
            this.cbDMask.AutoSize = true;
            this.cbDMask.Checked = true;
            this.cbDMask.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbDMask.Location = new System.Drawing.Point(220, 108);
            this.cbDMask.Name = "cbDMask";
            this.cbDMask.Size = new System.Drawing.Size(72, 16);
            this.cbDMask.TabIndex = 9;
            this.cbDMask.Text = "Dir. Mask";
            this.cbDMask.UseVisualStyleBackColor = true;
            // 
            // cbForceCom
            // 
            this.cbForceCom.AutoSize = true;
            this.cbForceCom.Checked = true;
            this.cbForceCom.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbForceCom.Location = new System.Drawing.Point(302, 108);
            this.cbForceCom.Name = "cbForceCom";
            this.cbForceCom.Size = new System.Drawing.Size(72, 16);
            this.cbForceCom.TabIndex = 9;
            this.cbForceCom.Text = "ForceCompress";
            this.cbForceCom.UseVisualStyleBackColor = true;
            // 
            // cbMode
            // 
            this.cbMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbMode.FormattingEnabled = true;
            this.cbMode.Items.AddRange(new object[] {
            "FILENAME",
            "ID",
            "FULL"});
            this.cbMode.Location = new System.Drawing.Point(79, 106);
            this.cbMode.Name = "cbMode";
            this.cbMode.Size = new System.Drawing.Size(121, 20);
            this.cbMode.TabIndex = 8;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(44, 109);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(29, 12);
            this.label6.TabIndex = 7;
            this.label6.Text = "Mode";
            // 
            // cbCode
            // 
            this.cbCode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbCode.FormattingEnabled = true;
            this.cbCode.Items.AddRange(new object[] {
            "UTF-8",
            "SJIS",
            "EUC"});
            this.cbCode.Location = new System.Drawing.Point(253, 71);
            this.cbCode.Name = "cbCode";
            this.cbCode.Size = new System.Drawing.Size(121, 20);
            this.cbCode.TabIndex = 6;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(218, 74);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "Coding";
            // 
            // numDataAlign
            // 
            this.numDataAlign.Location = new System.Drawing.Point(79, 70);
            this.numDataAlign.Maximum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.numDataAlign.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numDataAlign.Name = "numDataAlign";
            this.numDataAlign.Size = new System.Drawing.Size(78, 21);
            this.numDataAlign.TabIndex = 4;
            this.numDataAlign.Value = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(20, 74);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "Data Align";
            // 
            // tbPackPath
            // 
            this.tbPackPath.Location = new System.Drawing.Point(79, 35);
            this.tbPackPath.Name = "tbPackPath";
            this.tbPackPath.ReadOnly = true;
            this.tbPackPath.Size = new System.Drawing.Size(236, 21);
            this.tbPackPath.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 38);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "Data Path";
            // 
            // btnSaveCPK
            // 
            this.btnSaveCPK.Location = new System.Drawing.Point(321, 139);
            this.btnSaveCPK.Name = "btnSaveCPK";
            this.btnSaveCPK.Size = new System.Drawing.Size(53, 23);
            this.btnSaveCPK.TabIndex = 2;
            this.btnSaveCPK.Text = "Browse";
            this.btnSaveCPK.UseVisualStyleBackColor = true;
            this.btnSaveCPK.Click += new System.EventHandler(this.btnSaveCPK_Click);
            // 
            // btnPackPath
            // 
            this.btnPackPath.Location = new System.Drawing.Point(321, 33);
            this.btnPackPath.Name = "btnPackPath";
            this.btnPackPath.Size = new System.Drawing.Size(53, 23);
            this.btnPackPath.TabIndex = 2;
            this.btnPackPath.Text = "Browse";
            this.btnPackPath.UseVisualStyleBackColor = true;
            this.btnPackPath.Click += new System.EventHandler(this.btnPackPath_Click);
            // 
            // saveCPKDialog
            // 
            this.saveCPKDialog.DefaultExt = "cpk";
            this.saveCPKDialog.Filter = "CPK File|*.cpk";
            this.saveCPKDialog.Title = "Please choose the file name";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(419, 366);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Alpha Localization Team CPK Tools";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numDataAlign)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnBECpkFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbECpkFile;
        private System.Windows.Forms.Button btnExtract;
        private System.Windows.Forms.TextBox tbEPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEPath;
        private System.Windows.Forms.OpenFileDialog openCPKDialog;
        private System.Windows.Forms.FolderBrowserDialog saveExtractFolder;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbPackPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnPackPath;
        private System.Windows.Forms.ComboBox cbCode;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numDataAlign;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox cbForceCom;
        private System.Windows.Forms.ComboBox cbMode;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnCompress;
        private System.Windows.Forms.TextBox tbSaveCPK;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btnSaveCPK;
        private System.Windows.Forms.SaveFileDialog saveCPKDialog;
        private System.Windows.Forms.CheckBox cbDMask;
    }
}

