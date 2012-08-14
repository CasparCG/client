namespace Caspar_Pilot.Controls
{
    partial class HotkeyEditControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cbShift = new System.Windows.Forms.CheckBox();
            this.cbCtrl = new System.Windows.Forms.CheckBox();
            this.cbAlt = new System.Windows.Forms.CheckBox();
            this.cKey = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // cbShift
            // 
            this.cbShift.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbShift.AutoSize = true;
            this.cbShift.Location = new System.Drawing.Point(65, 2);
            this.cbShift.Name = "cbShift";
            this.cbShift.Size = new System.Drawing.Size(47, 17);
            this.cbShift.TabIndex = 0;
            this.cbShift.Text = "Shift";
            this.cbShift.UseVisualStyleBackColor = true;
            // 
            // cbCtrl
            // 
            this.cbCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbCtrl.AutoSize = true;
            this.cbCtrl.Location = new System.Drawing.Point(110, 2);
            this.cbCtrl.Name = "cbCtrl";
            this.cbCtrl.Size = new System.Drawing.Size(41, 17);
            this.cbCtrl.TabIndex = 1;
            this.cbCtrl.Text = "Ctrl";
            this.cbCtrl.UseVisualStyleBackColor = true;
            // 
            // cbAlt
            // 
            this.cbAlt.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbAlt.AutoSize = true;
            this.cbAlt.Location = new System.Drawing.Point(149, 2);
            this.cbAlt.Name = "cbAlt";
            this.cbAlt.Size = new System.Drawing.Size(38, 17);
            this.cbAlt.TabIndex = 2;
            this.cbAlt.Text = "Alt";
            this.cbAlt.UseVisualStyleBackColor = true;
            // 
            // cKey
            // 
            this.cKey.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cKey.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cKey.FormattingEnabled = true;
            this.cKey.Location = new System.Drawing.Point(0, 0);
            this.cKey.Name = "cKey";
            this.cKey.Size = new System.Drawing.Size(60, 21);
            this.cKey.TabIndex = 3;
            this.cKey.SelectedIndexChanged += new System.EventHandler(this.cKey_SelectedIndexChanged);
            // 
            // HotkeyEditControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cKey);
            this.Controls.Add(this.cbAlt);
            this.Controls.Add(this.cbCtrl);
            this.Controls.Add(this.cbShift);
            this.MinimumSize = new System.Drawing.Size(184, 21);
            this.Name = "HotkeyEditControl";
            this.Size = new System.Drawing.Size(184, 21);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox cbShift;
        private System.Windows.Forms.CheckBox cbCtrl;
        private System.Windows.Forms.CheckBox cbAlt;
        private System.Windows.Forms.ComboBox cKey;
    }
}
