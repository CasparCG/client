namespace Caspar_Pilot
{
	partial class OperatorForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OperatorForm));
            this.gbRundown_ = new System.Windows.Forms.GroupBox();
            this.tsRundown_ = new System.Windows.Forms.ToolStrip();
            this.btnNew_ = new System.Windows.Forms.ToolStripButton();
            this.btnDelete_ = new System.Windows.Forms.ToolStripButton();
            this.btnLoad_ = new System.Windows.Forms.ToolStripButton();
            this.btnPlay_ = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.btnUpdate_ = new System.Windows.Forms.ToolStripButton();
            this.btnStop_ = new System.Windows.Forms.ToolStripButton();
            this.btnOnAir_ = new System.Windows.Forms.ToolStripButton();
            this.btnClear_ = new System.Windows.Forms.ToolStripSplitButton();
            this.btnClearCG_ = new System.Windows.Forms.ToolStripMenuItem();
            this.btnClearPreRendered_ = new System.Windows.Forms.ToolStripMenuItem();
            this.btnClearAll_ = new System.Windows.Forms.ToolStripMenuItem();
            this.btnDetail_ = new System.Windows.Forms.ToolStripButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.pOnAir_ = new System.Windows.Forms.Panel();
            this.pItemLoaded_ = new System.Windows.Forms.Panel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.miFile_ = new System.Windows.Forms.ToolStripMenuItem();
            this.miNew_ = new System.Windows.Forms.ToolStripMenuItem();
            this.miLoad_ = new System.Windows.Forms.ToolStripMenuItem();
            this.miSave_ = new System.Windows.Forms.ToolStripMenuItem();
            this.miSaveAs_ = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.miSettings_ = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.miExit_ = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.compactModeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.togglePreviewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toggleFullscreenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.templateDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.outputSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.casparToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btnRefreshTemplates_ = new System.Windows.Forms.ToolStripMenuItem();
            this.btnRefreshGraphics_ = new System.Windows.Forms.ToolStripMenuItem();
            this.tankaMallgrafiklistaToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.itemLoadTimer_ = new System.Windows.Forms.Timer(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.miCasparStatus_ = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelPage = new System.Windows.Forms.ToolStripStatusLabel();
            this.gbRundown_.SuspendLayout();
            this.tsRundown_.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.pOnAir_.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbRundown_
            // 
            this.gbRundown_.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbRundown_.Controls.Add(this.tsRundown_);
            this.gbRundown_.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gbRundown_.Location = new System.Drawing.Point(4, 4);
            this.gbRundown_.Margin = new System.Windows.Forms.Padding(4);
            this.gbRundown_.MinimumSize = new System.Drawing.Size(365, 0);
            this.gbRundown_.Name = "gbRundown_";
            this.gbRundown_.Padding = new System.Windows.Forms.Padding(4);
            this.gbRundown_.Size = new System.Drawing.Size(463, 524);
            this.gbRundown_.TabIndex = 1;
            this.gbRundown_.TabStop = false;
            this.gbRundown_.Text = "Rundown";
            // 
            // tsRundown_
            // 
            this.tsRundown_.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tsRundown_.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tsRundown_.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnNew_,
            this.btnDelete_,
            this.btnLoad_,
            this.btnPlay_,
            this.toolStripButton1,
            this.btnUpdate_,
            this.btnStop_,
            this.btnOnAir_,
            this.btnClear_,
            this.btnDetail_});
            this.tsRundown_.Location = new System.Drawing.Point(4, 19);
            this.tsRundown_.Name = "tsRundown_";
            this.tsRundown_.Size = new System.Drawing.Size(455, 25);
            this.tsRundown_.TabIndex = 2;
            // 
            // btnNew_
            // 
            this.btnNew_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnNew_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnNew_.Name = "btnNew_";
            this.btnNew_.Size = new System.Drawing.Size(39, 22);
            this.btnNew_.Text = "New";
            this.btnNew_.Click += new System.EventHandler(this.btnNew__Click);
            // 
            // btnDelete_
            // 
            this.btnDelete_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnDelete_.Enabled = false;
            this.btnDelete_.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.btnDelete_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDelete_.Name = "btnDelete_";
            this.btnDelete_.Size = new System.Drawing.Size(52, 22);
            this.btnDelete_.Text = "Delete";
            this.btnDelete_.Click += new System.EventHandler(this.btnDelete__Click);
            // 
            // btnLoad_
            // 
            this.btnLoad_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnLoad_.Enabled = false;
            this.btnLoad_.Image = ((System.Drawing.Image)(resources.GetObject("btnLoad_.Image")));
            this.btnLoad_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnLoad_.Name = "btnLoad_";
            this.btnLoad_.Size = new System.Drawing.Size(43, 22);
            this.btnLoad_.Text = "Load";
            this.btnLoad_.Click += new System.EventHandler(this.btnLoad__Click);
            // 
            // btnPlay_
            // 
            this.btnPlay_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnPlay_.Enabled = false;
            this.btnPlay_.Image = ((System.Drawing.Image)(resources.GetObject("btnPlay_.Image")));
            this.btnPlay_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPlay_.Name = "btnPlay_";
            this.btnPlay_.Size = new System.Drawing.Size(39, 22);
            this.btnPlay_.Text = "Play";
            this.btnPlay_.Click += new System.EventHandler(this.btnPlay__Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButton1.Enabled = false;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(40, 22);
            this.toolStripButton1.Text = "Step";
            this.toolStripButton1.Click += new System.EventHandler(this.btnStep__Click);
            // 
            // btnUpdate_
            // 
            this.btnUpdate_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnUpdate_.Enabled = false;
            this.btnUpdate_.Image = ((System.Drawing.Image)(resources.GetObject("btnUpdate_.Image")));
            this.btnUpdate_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnUpdate_.Name = "btnUpdate_";
            this.btnUpdate_.Size = new System.Drawing.Size(57, 22);
            this.btnUpdate_.Text = "Update";
            this.btnUpdate_.Click += new System.EventHandler(this.btnUpdate__Click);
            // 
            // btnStop_
            // 
            this.btnStop_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnStop_.Enabled = false;
            this.btnStop_.Image = ((System.Drawing.Image)(resources.GetObject("btnStop_.Image")));
            this.btnStop_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStop_.Name = "btnStop_";
            this.btnStop_.Size = new System.Drawing.Size(40, 22);
            this.btnStop_.Text = "Stop";
            this.btnStop_.Click += new System.EventHandler(this.btnStop__Click);
            // 
            // btnOnAir_
            // 
            this.btnOnAir_.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.btnOnAir_.CheckOnClick = true;
            this.btnOnAir_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnOnAir_.Enabled = false;
            this.btnOnAir_.Image = ((System.Drawing.Image)(resources.GetObject("btnOnAir_.Image")));
            this.btnOnAir_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnOnAir_.Name = "btnOnAir_";
            this.btnOnAir_.Size = new System.Drawing.Size(47, 22);
            this.btnOnAir_.Text = "Off Air";
            this.btnOnAir_.CheckedChanged += new System.EventHandler(this.btnOnAir__CheckedChanged);
            // 
            // btnClear_
            // 
            this.btnClear_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnClear_.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnClearCG_,
            this.btnClearPreRendered_,
            this.btnClearAll_});
            this.btnClear_.Image = ((System.Drawing.Image)(resources.GetObject("btnClear_.Image")));
            this.btnClear_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnClear_.Name = "btnClear_";
            this.btnClear_.Size = new System.Drawing.Size(56, 22);
            this.btnClear_.Text = "Clear";
            this.btnClear_.ButtonClick += new System.EventHandler(this.btnClear__Click);
            // 
            // btnClearCG_
            // 
            this.btnClearCG_.Name = "btnClearCG_";
            this.btnClearCG_.Size = new System.Drawing.Size(218, 22);
            this.btnClearCG_.Tag = "CG";
            this.btnClearCG_.Text = "Clear template graphics";
            this.btnClearCG_.Click += new System.EventHandler(this.btnClear__Click);
            // 
            // btnClearPreRendered_
            // 
            this.btnClearPreRendered_.Name = "btnClearPreRendered_";
            this.btnClearPreRendered_.Size = new System.Drawing.Size(218, 22);
            this.btnClearPreRendered_.Tag = "PRE";
            this.btnClearPreRendered_.Text = "Clear graphics";
            this.btnClearPreRendered_.Click += new System.EventHandler(this.btnClear__Click);
            // 
            // btnClearAll_
            // 
            this.btnClearAll_.Name = "btnClearAll_";
            this.btnClearAll_.Size = new System.Drawing.Size(218, 22);
            this.btnClearAll_.Tag = "ALL";
            this.btnClearAll_.Text = "Clear all";
            this.btnClearAll_.Click += new System.EventHandler(this.btnClear__Click);
            // 
            // btnDetail_
            // 
            this.btnDetail_.CheckOnClick = true;
            this.btnDetail_.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnDetail_.Enabled = false;
            this.btnDetail_.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDetail_.Name = "btnDetail_";
            this.btnDetail_.Size = new System.Drawing.Size(47, 20);
            this.btnDetail_.Text = "Detalj";
            this.btnDetail_.Visible = false;
            this.btnDetail_.Click += new System.EventHandler(this.btnDetail__Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 16F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 67F));
            this.tableLayoutPanel1.Controls.Add(this.gbRundown_, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.pOnAir_, 1, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 24);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(4);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1444, 532);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // pOnAir_
            // 
            this.pOnAir_.Controls.Add(this.pItemLoaded_);
            this.pOnAir_.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pOnAir_.Location = new System.Drawing.Point(471, 0);
            this.pOnAir_.Margin = new System.Windows.Forms.Padding(0);
            this.pOnAir_.Name = "pOnAir_";
            this.pOnAir_.Size = new System.Drawing.Size(16, 532);
            this.pOnAir_.TabIndex = 2;
            // 
            // pItemLoaded_
            // 
            this.pItemLoaded_.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pItemLoaded_.Location = new System.Drawing.Point(5, 510);
            this.pItemLoaded_.Margin = new System.Windows.Forms.Padding(4);
            this.pItemLoaded_.Name = "pItemLoaded_";
            this.pItemLoaded_.Size = new System.Drawing.Size(8, 17);
            this.pItemLoaded_.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miFile_,
            this.viewToolStripMenuItem,
            this.casparToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(8, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(1444, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // miFile_
            // 
            this.miFile_.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miNew_,
            this.miLoad_,
            this.miSave_,
            this.miSaveAs_,
            this.toolStripSeparator2,
            this.miSettings_,
            this.toolStripSeparator3,
            this.miExit_});
            this.miFile_.Name = "miFile_";
            this.miFile_.Size = new System.Drawing.Size(42, 20);
            this.miFile_.Text = "&File";
            this.miFile_.DropDownOpening += new System.EventHandler(this.miArkiv__DropDownOpening);
            // 
            // miNew_
            // 
            this.miNew_.Name = "miNew_";
            this.miNew_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.miNew_.Size = new System.Drawing.Size(172, 22);
            this.miNew_.Text = "&New";
            this.miNew_.Click += new System.EventHandler(this.miNew__Click);
            // 
            // miLoad_
            // 
            this.miLoad_.Name = "miLoad_";
            this.miLoad_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.miLoad_.Size = new System.Drawing.Size(172, 22);
            this.miLoad_.Text = "&Open...";
            this.miLoad_.Click += new System.EventHandler(this.miLoad__Click);
            // 
            // miSave_
            // 
            this.miSave_.Name = "miSave_";
            this.miSave_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.miSave_.Size = new System.Drawing.Size(172, 22);
            this.miSave_.Text = "&Save";
            this.miSave_.Click += new System.EventHandler(this.miSave__Click);
            // 
            // miSaveAs_
            // 
            this.miSaveAs_.Name = "miSaveAs_";
            this.miSaveAs_.Size = new System.Drawing.Size(172, 22);
            this.miSaveAs_.Text = "S&ave as...";
            this.miSaveAs_.Click += new System.EventHandler(this.miSaveAs__Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(169, 6);
            // 
            // miSettings_
            // 
            this.miSettings_.Name = "miSettings_";
            this.miSettings_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.S)));
            this.miSettings_.Size = new System.Drawing.Size(172, 22);
            this.miSettings_.Text = "Se&ttings...";
            this.miSettings_.Click += new System.EventHandler(this.miSettings__Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(169, 6);
            // 
            // miExit_
            // 
            this.miExit_.Name = "miExit_";
            this.miExit_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.E)));
            this.miExit_.Size = new System.Drawing.Size(172, 22);
            this.miExit_.Text = "&Exit";
            this.miExit_.Click += new System.EventHandler(this.miExit__Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.compactModeToolStripMenuItem,
            this.toolStripSeparator4,
            this.togglePreviewToolStripMenuItem,
            this.toggleFullscreenToolStripMenuItem,
            this.templateDataToolStripMenuItem,
            this.outputSettingsToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.T)));
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // compactModeToolStripMenuItem
            // 
            this.compactModeToolStripMenuItem.Name = "compactModeToolStripMenuItem";
            this.compactModeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.D)));
            this.compactModeToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.compactModeToolStripMenuItem.Text = "Compact &mode";
            this.compactModeToolStripMenuItem.Click += new System.EventHandler(this.compactModeToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(249, 6);
            // 
            // togglePreviewToolStripMenuItem
            // 
            this.togglePreviewToolStripMenuItem.Name = "togglePreviewToolStripMenuItem";
            this.togglePreviewToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.togglePreviewToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.togglePreviewToolStripMenuItem.Text = "Toggle &preview";
            this.togglePreviewToolStripMenuItem.Click += new System.EventHandler(this.togglePreviewToolStripMenuItem_Click);
            // 
            // toggleFullscreenToolStripMenuItem
            // 
            this.toggleFullscreenToolStripMenuItem.Name = "toggleFullscreenToolStripMenuItem";
            this.toggleFullscreenToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.toggleFullscreenToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.toggleFullscreenToolStripMenuItem.Text = "Toggle &fullscreen";
            this.toggleFullscreenToolStripMenuItem.Click += new System.EventHandler(this.toggleFullscreenToolStripMenuItem_Click);
            // 
            // templateDataToolStripMenuItem
            // 
            this.templateDataToolStripMenuItem.Name = "templateDataToolStripMenuItem";
            this.templateDataToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.T)));
            this.templateDataToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.templateDataToolStripMenuItem.Text = "Toggle template &data";
            this.templateDataToolStripMenuItem.Click += new System.EventHandler(this.templateDataToolStripMenuItem_Click);
            // 
            // outputSettingsToolStripMenuItem
            // 
            this.outputSettingsToolStripMenuItem.Name = "outputSettingsToolStripMenuItem";
            this.outputSettingsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.U)));
            this.outputSettingsToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.outputSettingsToolStripMenuItem.Text = "Toggle o&utput settings";
            this.outputSettingsToolStripMenuItem.Click += new System.EventHandler(this.outputSettingsToolStripMenuItem_Click);
            // 
            // casparToolStripMenuItem
            // 
            this.casparToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnRefreshTemplates_,
            this.btnRefreshGraphics_,
            this.tankaMallgrafiklistaToolStripMenuItem});
            this.casparToolStripMenuItem.Name = "casparToolStripMenuItem";
            this.casparToolStripMenuItem.Size = new System.Drawing.Size(64, 20);
            this.casparToolStripMenuItem.Text = "Caspar";
            // 
            // btnRefreshTemplates_
            // 
            this.btnRefreshTemplates_.Name = "btnRefreshTemplates_";
            this.btnRefreshTemplates_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.M)));
            this.btnRefreshTemplates_.Size = new System.Drawing.Size(328, 22);
            this.btnRefreshTemplates_.Text = "Reload template list";
            this.btnRefreshTemplates_.Click += new System.EventHandler(this.btnRefreshTemplates__Click);
            // 
            // btnRefreshGraphics_
            // 
            this.btnRefreshGraphics_.Name = "btnRefreshGraphics_";
            this.btnRefreshGraphics_.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.btnRefreshGraphics_.Size = new System.Drawing.Size(328, 22);
            this.btnRefreshGraphics_.Text = "Reload graphic list";
            this.btnRefreshGraphics_.Click += new System.EventHandler(this.btnRefreshGraphics__Click);
            // 
            // tankaMallgrafiklistaToolStripMenuItem
            // 
            this.tankaMallgrafiklistaToolStripMenuItem.Name = "tankaMallgrafiklistaToolStripMenuItem";
            this.tankaMallgrafiklistaToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.M)));
            this.tankaMallgrafiklistaToolStripMenuItem.Size = new System.Drawing.Size(328, 22);
            this.tankaMallgrafiklistaToolStripMenuItem.Text = "Reload templates from offline folder";
            this.tankaMallgrafiklistaToolStripMenuItem.Click += new System.EventHandler(this.tankaMallgrafiklistaToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.aboutToolStripMenuItem.Text = "&About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // itemLoadTimer_
            // 
            this.itemLoadTimer_.Interval = 500;
            this.itemLoadTimer_.Tick += new System.EventHandler(this.itemLoadTimer__Tick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miCasparStatus_,
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabelPage});
            this.statusStrip1.Location = new System.Drawing.Point(0, 556);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 19, 0);
            this.statusStrip1.Size = new System.Drawing.Size(1444, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // miCasparStatus_
            // 
            this.miCasparStatus_.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.miCasparStatus_.DoubleClickEnabled = true;
            this.miCasparStatus_.Image = global::Caspar_Pilot.Properties.Resources.disconnected;
            this.miCasparStatus_.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.miCasparStatus_.Name = "miCasparStatus_";
            this.miCasparStatus_.Size = new System.Drawing.Size(112, 22);
            this.miCasparStatus_.Text = "Server status";
            this.miCasparStatus_.DoubleClick += new System.EventHandler(this.miCasparStatus__DoubleClick);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(1190, 17);
            this.toolStripStatusLabel1.Spring = true;
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(41, 17);
            this.toolStripStatusLabel2.Text = "Page:";
            // 
            // toolStripStatusLabelPage
            // 
            this.toolStripStatusLabelPage.AutoSize = false;
            this.toolStripStatusLabelPage.Name = "toolStripStatusLabelPage";
            this.toolStripStatusLabelPage.Size = new System.Drawing.Size(50, 17);
            this.toolStripStatusLabelPage.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // OperatorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1444, 578);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "OperatorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CasparCG Client";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OperatorForm_KeyPress);
            this.gbRundown_.ResumeLayout(false);
            this.gbRundown_.PerformLayout();
            this.tsRundown_.ResumeLayout(false);
            this.tsRundown_.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.pOnAir_.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ToolStrip tsRundown_;
		private System.Windows.Forms.ToolStripButton btnPlay_;
		private System.Windows.Forms.ToolStripButton btnStop_;
		private System.Windows.Forms.GroupBox gbRundown_;
		private System.Windows.Forms.ToolStripButton btnLoad_;
		private System.Windows.Forms.ToolStripButton btnOnAir_;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem miFile_;
		private System.Windows.Forms.ToolStripMenuItem miLoad_;
		private System.Windows.Forms.ToolStripMenuItem miSaveAs_;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripMenuItem miSettings_;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripMenuItem miExit_;
        private System.Windows.Forms.ToolStripMenuItem miNew_;
		private System.Windows.Forms.ToolStripMenuItem miSave_;
		private System.Windows.Forms.Timer itemLoadTimer_;
		private System.Windows.Forms.Panel pOnAir_;
		private System.Windows.Forms.Panel pItemLoaded_;
		private System.Windows.Forms.ToolStripSplitButton btnClear_;
		private System.Windows.Forms.ToolStripMenuItem btnClearCG_;
		private System.Windows.Forms.ToolStripMenuItem btnClearAll_;
		private System.Windows.Forms.ToolStripMenuItem btnClearPreRendered_;
		private System.Windows.Forms.ToolStripMenuItem casparToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem btnRefreshTemplates_;
		private System.Windows.Forms.ToolStripMenuItem btnRefreshGraphics_;
		private System.Windows.Forms.ToolStripButton btnUpdate_;
        private System.Windows.Forms.ToolStripMenuItem tankaMallgrafiklistaToolStripMenuItem;
       // private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btnNew_;
        private System.Windows.Forms.ToolStripButton btnDelete_;
        private System.Windows.Forms.ToolStripButton btnDetail_;
		private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toggleFullscreenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem miCasparStatus_;
        private System.Windows.Forms.ToolStripMenuItem togglePreviewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem compactModeToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelPage;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem templateDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem outputSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
	}
}

