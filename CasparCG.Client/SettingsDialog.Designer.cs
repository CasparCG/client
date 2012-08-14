namespace Caspar_Pilot
{
	partial class SettingsDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SettingsDialog));
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("Playout menu", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("Caspar menu", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem(new string[] {
            "Template",
            "LightBlue"}, -1);
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem(new string[] {
            "Pre-rendered",
            "GhostWhite"}, -1);
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem(new string[] {
            "Multistep template",
            "Gold"}, -1);
            this.btnCancel_ = new System.Windows.Forms.Button();
            this.btnOK_ = new System.Windows.Forms.Button();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btnAddHost_ = new System.Windows.Forms.ToolStripButton();
            this.btnSaveHost_ = new System.Windows.Forms.ToolStripButton();
            this.btnDeleteHost_ = new System.Windows.Forms.ToolStripButton();
            this.lHosts_ = new System.Windows.Forms.ListBox();
            this.panColorpick = new System.Windows.Forms.Panel();
            this.nEditHostPort_ = new System.Windows.Forms.NumericUpDown();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.nEditHostChannel_ = new System.Windows.Forms.NumericUpDown();
            this.tEditHostname_ = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.tEditHostTitle_ = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.btnRestoreDefaultKeys = new System.Windows.Forms.Button();
            this.lvHotKey_ = new Caspar_Pilot.ListViewEx();
            this.Actions = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.HotKey = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.cbColor_ = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.txtColor_ = new System.Windows.Forms.TextBox();
            this.lvColor_ = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.nFieldStartIndex = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.tfieldPrefix = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.pBGColor = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnBrowse_ = new System.Windows.Forms.Button();
            this.tTemplateFolder_ = new System.Windows.Forms.TextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage6.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nEditHostPort_)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nEditHostChannel_)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldStartIndex)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel_
            // 
            this.btnCancel_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel_.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel_.Location = new System.Drawing.Point(300, 436);
            this.btnCancel_.Margin = new System.Windows.Forms.Padding(4);
            this.btnCancel_.Name = "btnCancel_";
            this.btnCancel_.Size = new System.Drawing.Size(95, 26);
            this.btnCancel_.TabIndex = 2;
            this.btnCancel_.Text = "&Cancel";
            this.btnCancel_.UseVisualStyleBackColor = true;
            // 
            // btnOK_
            // 
            this.btnOK_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOK_.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK_.Location = new System.Drawing.Point(195, 436);
            this.btnOK_.Margin = new System.Windows.Forms.Padding(4);
            this.btnOK_.Name = "btnOK_";
            this.btnOK_.Size = new System.Drawing.Size(95, 26);
            this.btnOK_.TabIndex = 3;
            this.btnOK_.Text = "&OK";
            this.btnOK_.UseVisualStyleBackColor = true;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Actions";
            this.columnHeader1.Width = 133;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "HotKey";
            this.columnHeader2.Width = 83;
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.groupBox1);
            this.tabPage6.Location = new System.Drawing.Point(4, 25);
            this.tabPage6.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Size = new System.Drawing.Size(371, 385);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "Outputs";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.toolStrip1);
            this.groupBox1.Controls.Add(this.lHosts_);
            this.groupBox1.Controls.Add(this.panColorpick);
            this.groupBox1.Controls.Add(this.nEditHostPort_);
            this.groupBox1.Controls.Add(this.label10);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.nEditHostChannel_);
            this.groupBox1.Controls.Add(this.tEditHostname_);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.tEditHostTitle_);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Location = new System.Drawing.Point(4, 4);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox1.Size = new System.Drawing.Size(360, 377);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Hosts";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnAddHost_,
            this.btnSaveHost_,
            this.btnDeleteHost_});
            this.toolStrip1.Location = new System.Drawing.Point(4, 126);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(352, 31);
            this.toolStrip1.TabIndex = 16;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btnAddHost_
            // 
            this.btnAddHost_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAddHost_.Enabled = false;
            this.btnAddHost_.Image = ((System.Drawing.Image)(resources.GetObject("btnAddHost_.Image")));
            this.btnAddHost_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAddHost_.Name = "btnAddHost_";
            this.btnAddHost_.Size = new System.Drawing.Size(37, 28);
            this.btnAddHost_.Text = "Add";
            this.btnAddHost_.Click += new System.EventHandler(this.btnAddHost__Click);
            // 
            // btnSaveHost_
            // 
            this.btnSaveHost_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnSaveHost_.Enabled = false;
            this.btnSaveHost_.Image = ((System.Drawing.Image)(resources.GetObject("btnSaveHost_.Image")));
            this.btnSaveHost_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnSaveHost_.Name = "btnSaveHost_";
            this.btnSaveHost_.Size = new System.Drawing.Size(44, 28);
            this.btnSaveHost_.Text = "Save";
            this.btnSaveHost_.Click += new System.EventHandler(this.btnSaveHost__Click);
            // 
            // btnDeleteHost_
            // 
            this.btnDeleteHost_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnDeleteHost_.Enabled = false;
            this.btnDeleteHost_.Image = ((System.Drawing.Image)(resources.GetObject("btnDeleteHost_.Image")));
            this.btnDeleteHost_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDeleteHost_.Name = "btnDeleteHost_";
            this.btnDeleteHost_.Size = new System.Drawing.Size(64, 28);
            this.btnDeleteHost_.Text = "Remove";
            this.btnDeleteHost_.Click += new System.EventHandler(this.btnDeleteHost__Click);
            // 
            // lHosts_
            // 
            this.lHosts_.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lHosts_.FormattingEnabled = true;
            this.lHosts_.IntegralHeight = false;
            this.lHosts_.ItemHeight = 16;
            this.lHosts_.Location = new System.Drawing.Point(4, 156);
            this.lHosts_.Margin = new System.Windows.Forms.Padding(4);
            this.lHosts_.Name = "lHosts_";
            this.lHosts_.Size = new System.Drawing.Size(351, 216);
            this.lHosts_.TabIndex = 15;
            this.lHosts_.SelectedIndexChanged += new System.EventHandler(this.lHosts__SelectedIndexChanged);
            // 
            // panColorpick
            // 
            this.panColorpick.BackColor = System.Drawing.Color.White;
            this.panColorpick.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panColorpick.Location = new System.Drawing.Point(296, 22);
            this.panColorpick.Margin = new System.Windows.Forms.Padding(4);
            this.panColorpick.Name = "panColorpick";
            this.panColorpick.Size = new System.Drawing.Size(55, 22);
            this.panColorpick.TabIndex = 14;
            this.panColorpick.Click += new System.EventHandler(this.panColorpick_Click);
            // 
            // nEditHostPort_
            // 
            this.nEditHostPort_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nEditHostPort_.Location = new System.Drawing.Point(200, 76);
            this.nEditHostPort_.Margin = new System.Windows.Forms.Padding(4);
            this.nEditHostPort_.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.nEditHostPort_.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nEditHostPort_.Name = "nEditHostPort_";
            this.nEditHostPort_.Size = new System.Drawing.Size(88, 22);
            this.nEditHostPort_.TabIndex = 13;
            this.nEditHostPort_.Value = new decimal(new int[] {
            5250,
            0,
            0,
            0});
            // 
            // label10
            // 
            this.label10.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(196, 57);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(35, 16);
            this.label10.TabIndex = 12;
            this.label10.Text = "Port:";
            // 
            // label9
            // 
            this.label9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(292, 57);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(60, 16);
            this.label9.TabIndex = 11;
            this.label9.Text = "Channel:";
            // 
            // nEditHostChannel_
            // 
            this.nEditHostChannel_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nEditHostChannel_.Location = new System.Drawing.Point(296, 76);
            this.nEditHostChannel_.Margin = new System.Windows.Forms.Padding(4);
            this.nEditHostChannel_.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nEditHostChannel_.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nEditHostChannel_.Name = "nEditHostChannel_";
            this.nEditHostChannel_.Size = new System.Drawing.Size(56, 22);
            this.nEditHostChannel_.TabIndex = 10;
            this.nEditHostChannel_.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // tEditHostname_
            // 
            this.tEditHostname_.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tEditHostname_.Location = new System.Drawing.Point(8, 76);
            this.tEditHostname_.Margin = new System.Windows.Forms.Padding(4);
            this.tEditHostname_.Name = "tEditHostname_";
            this.tEditHostname_.Size = new System.Drawing.Size(183, 22);
            this.tEditHostname_.TabIndex = 3;
            this.tEditHostname_.TextChanged += new System.EventHandler(this.tEditHostTextbox__TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(4, 57);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(95, 16);
            this.label8.TabIndex = 2;
            this.label8.Text = "Hostname / IP:";
            // 
            // tEditHostTitle_
            // 
            this.tEditHostTitle_.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tEditHostTitle_.Location = new System.Drawing.Point(63, 22);
            this.tEditHostTitle_.Margin = new System.Windows.Forms.Padding(4);
            this.tEditHostTitle_.Name = "tEditHostTitle_";
            this.tEditHostTitle_.Size = new System.Drawing.Size(224, 22);
            this.tEditHostTitle_.TabIndex = 1;
            this.tEditHostTitle_.TextChanged += new System.EventHandler(this.tEditHostTextbox__TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(4, 26);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(48, 16);
            this.label7.TabIndex = 0;
            this.label7.Text = "Name:";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.btnRestoreDefaultKeys);
            this.tabPage3.Controls.Add(this.lvHotKey_);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(371, 385);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Hotkeys";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // btnRestoreDefaultKeys
            // 
            this.btnRestoreDefaultKeys.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRestoreDefaultKeys.Location = new System.Drawing.Point(244, 6);
            this.btnRestoreDefaultKeys.Margin = new System.Windows.Forms.Padding(4);
            this.btnRestoreDefaultKeys.Name = "btnRestoreDefaultKeys";
            this.btnRestoreDefaultKeys.Size = new System.Drawing.Size(120, 24);
            this.btnRestoreDefaultKeys.TabIndex = 10;
            this.btnRestoreDefaultKeys.Text = "&Restore Defaults";
            this.btnRestoreDefaultKeys.UseVisualStyleBackColor = true;
            this.btnRestoreDefaultKeys.Visible = false;
            // 
            // lvHotKey_
            // 
            this.lvHotKey_.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.lvHotKey_.AllowColumnReorder = true;
            this.lvHotKey_.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lvHotKey_.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Actions,
            this.HotKey});
            this.lvHotKey_.DoubleClickActivation = true;
            this.lvHotKey_.FullRowSelect = true;
            listViewGroup1.Header = "Playout menu";
            listViewGroup1.Name = "listViewGroup1";
            listViewGroup2.Header = "Caspar menu";
            listViewGroup2.Name = "listViewGroup2";
            this.lvHotKey_.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1,
            listViewGroup2});
            this.lvHotKey_.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lvHotKey_.Location = new System.Drawing.Point(4, 38);
            this.lvHotKey_.Margin = new System.Windows.Forms.Padding(4);
            this.lvHotKey_.MultiSelect = false;
            this.lvHotKey_.Name = "lvHotKey_";
            this.lvHotKey_.ShowItemToolTips = true;
            this.lvHotKey_.Size = new System.Drawing.Size(360, 343);
            this.lvHotKey_.TabIndex = 0;
            this.lvHotKey_.UseCompatibleStateImageBehavior = false;
            this.lvHotKey_.View = System.Windows.Forms.View.Details;
            // 
            // Actions
            // 
            this.Actions.Text = "Actions";
            this.Actions.Width = 110;
            // 
            // HotKey
            // 
            this.HotKey.Text = "HotKey";
            this.HotKey.Width = 246;
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.cbColor_);
            this.tabPage4.Controls.Add(this.button1);
            this.tabPage4.Controls.Add(this.txtColor_);
            this.tabPage4.Controls.Add(this.lvColor_);
            this.tabPage4.Location = new System.Drawing.Point(4, 25);
            this.tabPage4.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(371, 385);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Colors";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // cbColor_
            // 
            this.cbColor_.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cbColor_.FormattingEnabled = true;
            this.cbColor_.Items.AddRange(new object[] {
            "Red",
            "Orange",
            "Yellow",
            "Green",
            "Gray",
            "Brown",
            "White",
            "Gold",
            "Silver",
            "LightBlue",
            "LightGreen",
            "CornflowerBlue",
            "LightSalmon",
            "PaleTurquoise"});
            this.cbColor_.Location = new System.Drawing.Point(180, 344);
            this.cbColor_.Margin = new System.Windows.Forms.Padding(4);
            this.cbColor_.Name = "cbColor_";
            this.cbColor_.Size = new System.Drawing.Size(183, 24);
            this.cbColor_.TabIndex = 15;
            this.cbColor_.TextChanged += new System.EventHandler(this.cbColor__TextChanged);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(245, 6);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(119, 24);
            this.button1.TabIndex = 14;
            this.button1.Text = "&Restore Defaults";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // txtColor_
            // 
            this.txtColor_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.txtColor_.Location = new System.Drawing.Point(4, 345);
            this.txtColor_.Margin = new System.Windows.Forms.Padding(4);
            this.txtColor_.Name = "txtColor_";
            this.txtColor_.ReadOnly = true;
            this.txtColor_.Size = new System.Drawing.Size(159, 22);
            this.txtColor_.TabIndex = 12;
            // 
            // lvColor_
            // 
            this.lvColor_.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.lvColor_.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lvColor_.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.lvColor_.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3});
            this.lvColor_.LabelEdit = true;
            this.lvColor_.Location = new System.Drawing.Point(4, 36);
            this.lvColor_.Margin = new System.Windows.Forms.Padding(4);
            this.lvColor_.Name = "lvColor_";
            this.lvColor_.Size = new System.Drawing.Size(359, 300);
            this.lvColor_.TabIndex = 11;
            this.lvColor_.UseCompatibleStateImageBehavior = false;
            this.lvColor_.View = System.Windows.Forms.View.Details;
            this.lvColor_.SelectedIndexChanged += new System.EventHandler(this.lvColor__SelectedIndexChanged);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Item type";
            this.columnHeader3.Width = 171;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Color";
            this.columnHeader4.Width = 95;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox4);
            this.tabPage2.Controls.Add(this.groupBox3);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(4);
            this.tabPage2.Size = new System.Drawing.Size(371, 385);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Common";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.nFieldStartIndex);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.tfieldPrefix);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Location = new System.Drawing.Point(4, 148);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox4.Size = new System.Drawing.Size(360, 74);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Template data";
            // 
            // nFieldStartIndex
            // 
            this.nFieldStartIndex.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nFieldStartIndex.Location = new System.Drawing.Point(292, 32);
            this.nFieldStartIndex.Margin = new System.Windows.Forms.Padding(4);
            this.nFieldStartIndex.Name = "nFieldStartIndex";
            this.nFieldStartIndex.Size = new System.Drawing.Size(60, 22);
            this.nFieldStartIndex.TabIndex = 14;
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(214, 34);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(70, 16);
            this.label6.TabIndex = 13;
            this.label6.Text = "Startindex:";
            // 
            // tfieldPrefix
            // 
            this.tfieldPrefix.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tfieldPrefix.Location = new System.Drawing.Point(63, 31);
            this.tfieldPrefix.Margin = new System.Windows.Forms.Padding(4);
            this.tfieldPrefix.Name = "tfieldPrefix";
            this.tfieldPrefix.Size = new System.Drawing.Size(132, 22);
            this.tfieldPrefix.TabIndex = 12;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 34);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 16);
            this.label5.TabIndex = 11;
            this.label5.Text = "Prefix:";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.pBGColor);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.btnBrowse_);
            this.groupBox3.Controls.Add(this.tTemplateFolder_);
            this.groupBox3.Location = new System.Drawing.Point(4, 7);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox3.Size = new System.Drawing.Size(360, 133);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Preview";
            // 
            // pBGColor
            // 
            this.pBGColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pBGColor.Location = new System.Drawing.Point(12, 91);
            this.pBGColor.Margin = new System.Windows.Forms.Padding(4);
            this.pBGColor.Name = "pBGColor";
            this.pBGColor.Size = new System.Drawing.Size(107, 22);
            this.pBGColor.TabIndex = 7;
            this.pBGColor.DoubleClick += new System.EventHandler(this.pBGColor_DoubleClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 71);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(117, 16);
            this.label1.TabIndex = 6;
            this.label1.Text = "Background color:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 23);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(220, 16);
            this.label4.TabIndex = 3;
            this.label4.Text = "Offline folder with graphic templates:";
            // 
            // btnBrowse_
            // 
            this.btnBrowse_.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBrowse_.Location = new System.Drawing.Point(274, 42);
            this.btnBrowse_.Margin = new System.Windows.Forms.Padding(4);
            this.btnBrowse_.Name = "btnBrowse_";
            this.btnBrowse_.Size = new System.Drawing.Size(78, 24);
            this.btnBrowse_.TabIndex = 5;
            this.btnBrowse_.Text = "&Browse...";
            this.btnBrowse_.UseVisualStyleBackColor = true;
            this.btnBrowse_.Click += new System.EventHandler(this.btnBrowse__Click);
            // 
            // tTemplateFolder_
            // 
            this.tTemplateFolder_.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tTemplateFolder_.Location = new System.Drawing.Point(12, 43);
            this.tTemplateFolder_.Margin = new System.Windows.Forms.Padding(4);
            this.tTemplateFolder_.Name = "tTemplateFolder_";
            this.tTemplateFolder_.Size = new System.Drawing.Size(254, 22);
            this.tTemplateFolder_.TabIndex = 4;
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage6);
            this.tabControl1.Location = new System.Drawing.Point(16, 13);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(379, 414);
            this.tabControl1.TabIndex = 4;
            // 
            // SettingsDialog
            // 
            this.AcceptButton = this.btnOK_;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel_;
            this.ClientSize = new System.Drawing.Size(407, 473);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.btnOK_);
            this.Controls.Add(this.btnCancel_);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "SettingsDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Settings";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SettingsDialog_FormClosing);
            this.Load += new System.EventHandler(this.SettingsDialog_Load);
            this.tabPage6.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nEditHostPort_)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nEditHostChannel_)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldStartIndex)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

        private System.Windows.Forms.Button btnCancel_;
		private System.Windows.Forms.Button btnOK_;
        private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.TabPage tabPage6;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.NumericUpDown nEditHostPort_;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.NumericUpDown nEditHostChannel_;
		private System.Windows.Forms.TextBox tEditHostname_;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.TextBox tEditHostTitle_;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Button btnRestoreDefaultKeys;
		private ListViewEx lvHotKey_;
		private System.Windows.Forms.ColumnHeader Actions;
		private System.Windows.Forms.ColumnHeader HotKey;
		private System.Windows.Forms.TabPage tabPage4;
		private System.Windows.Forms.ComboBox cbColor_;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox txtColor_;
		private System.Windows.Forms.ListView lvColor_;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.NumericUpDown nFieldStartIndex;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox tfieldPrefix;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button btnBrowse_;
		private System.Windows.Forms.TextBox tTemplateFolder_;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.Panel panColorpick;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton btnAddHost_;
		private System.Windows.Forms.ToolStripButton btnSaveHost_;
		private System.Windows.Forms.ToolStripButton btnDeleteHost_;
		private System.Windows.Forms.ListBox lHosts_;
		private System.Windows.Forms.Panel pBGColor;
		private System.Windows.Forms.Label label1;
	}
}