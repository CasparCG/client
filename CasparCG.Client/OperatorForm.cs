using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.XPath;
using Caspar_Pilot.Clients;
using System.Reflection;
using System.Diagnostics;

namespace Caspar_Pilot
{
	public partial class OperatorForm : Form, IMessageFilter
	{
		private int copyIndex = -1;
        public string VersionString = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).FileVersion;

		private static string NewItemName = "[New event]";
		private delegate void UpdateGUI(object parameter);

		internal Hosts.HostsManager HostsManager { get; private set; }

		public delegate void StringDelegate(string name);
        Dictionary<string, Keys> hotKeys_ = new Dictionary<string, Keys>();

		DnDListBox<RundownItem> lbRundown_ = null;
        Caspar_Pilot.Controls.ItemViewControl itemViewControl_ = null;

        private string Page { get; set; }

		StringFormat channelStringFormat_;
		Brush fontBrushSelected_;
		Brush fontBrush_;

        private int windowWidth = -1;
        private bool IsCompactMode { get; set; }
        private bool IsEnterPressed { get; set; }

        public OperatorForm()
		{
			HostsManager = new Hosts.HostsManager();
            HostsManager.ValidConnectionsChanged += HostsManager_ValidConnectionsChanged;

			channelStringFormat_ = new StringFormat(StringFormatFlags.NoWrap);
			channelStringFormat_.Alignment = StringAlignment.Far;
			fontBrush_ = new SolidBrush(Color.Black);
			fontBrushSelected_ = new SolidBrush(Color.White);

            try
            {
               hotKeys_.Add("Load", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.LoadKey));
               hotKeys_.Add("Play", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.PlayKey));
               hotKeys_.Add("Step", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.StepKey));
               hotKeys_.Add("Stop", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.StopKey));
               hotKeys_.Add("Update", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.UpdateKey));
			   hotKeys_.Add("QuickPlay", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.QuickPlayKey));
			   hotKeys_.Add("QuickStop", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.QuickStopKey));
			   hotKeys_.Add("QuickStep", (Keys)Enum.Parse(typeof(Keys), Properties.Settings.Default.QuickStepKey));
			}
            catch (Exception)
			{}

			InitializeComponent();
			InitializeCustomComponents();
			UpdateTitle();

			IsCurrentItemLoaded = false;
		}

		private void InitializeCustomComponents()
		{
			//Left side: rundown
			{
				Control itemsMenu = gbRundown_.Controls[0];
				gbRundown_.Controls.Clear();

				lbRundown_ = new DnDListBox<RundownItem>();
				lbRundown_.SelectionMode = SelectionMode.One;
				lbRundown_.AllowDrop = true;
				lbRundown_.SelectedIndexChanged += new EventHandler(lbRundown__SelectedIndexChanged);
				lbRundown_.Dock = DockStyle.Fill;
				lbRundown_.IntegralHeight = false;
				lbRundown_.ItemHeight = 35;
				lbRundown_.DrawMode = DrawMode.OwnerDrawFixed;
				lbRundown_.DrawItem += new DrawItemEventHandler(lbRundown__DrawItem);
				lbRundown_.DragDrop += new DragEventHandler(lbRundown__DragDrop);
				lbRundown_.Resize += new EventHandler(lbRundown__Resize);
				lbRundown_.KeyDown += new KeyEventHandler(lbRundown__KeyDown);
				gbRundown_.Controls.Add(lbRundown_);
                gbRundown_.Controls.Add(itemsMenu);
			}

			//Right side: View the selected item
			{
				itemViewControl_ = new Caspar_Pilot.Controls.ItemViewControl();
				itemViewControl_.OperatorForm = this;
				itemViewControl_.Enabled = false;
				itemViewControl_.Dock = DockStyle.Fill;
				itemViewControl_.CurrentItemDataChanged += new EventHandler<EventArgs>(itemViewControl__CurrentItemDataChanged);
				itemViewControl_.PreviewControl.BackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;
				SetPreviewControlPaths();

				this.tableLayoutPanel1.Controls.Add(itemViewControl_, 2, 0);
			}

            this.itemViewControl_.EnableOutputSettings = Properties.Settings.Default.EnableOutputSettings;
            this.itemViewControl_.EnableTemplateData = Properties.Settings.Default.EnableTemplateData;
            this.itemViewControl_.EnableVideoLayers = Properties.Settings.Default.EnableVideoLayers;
            this.itemViewControl_.EnableControlValues = Properties.Settings.Default.EnableControlValues;
		}

		private void lbRundown__KeyDown(object sender, KeyEventArgs e)
		{
			if (!e.Control && e.KeyCode == Keys.C)
			{
				lbRundown_.SelectedIndex--;
			}
			else if (e.Control && e.KeyCode == Keys.C)
			{
				this.copyIndex = lbRundown_.SelectedIndex;

				// Workaround for the problem with c-key changes the selected index.
				if (lbRundown_.SelectedIndex == 0)
				{
					lbRundown_.SelectedIndex = lbRundown_.Items.Count - 1;
				}
				else
				{
					lbRundown_.SelectedIndex--;
				}
			}
			else if (e.Control && e.KeyCode == Keys.V)
			{
				CopySelectedItem();
			}
		}

		private void CopySelectedItem()
		{
			try
			{
				if (copyIndex >= 0)
				{
					RundownItem oldItem = lbRundown_.Items[this.copyIndex] as RundownItem;
					RundownItem newItem = oldItem.Clone() as RundownItem;

					lbRundown_.Items.Insert(lbRundown_.SelectedIndex, newItem);

					IsDirty = true;
				}
			}
			catch { }

		}

		private void SetPreviewControlPaths()
		{
			string templateFolder = Properties.Settings.Default.TemplateFolder;
			if (string.IsNullOrEmpty(templateFolder))
			{
				templateFolder = GetCasparTemplateFolder();
			}

			itemViewControl_.PreviewControl.TemplateFolder = templateFolder;

			//Try TemplateHost v1.8
			string templateHost = System.IO.Path.Combine((string.IsNullOrEmpty(templateFolder)) ? Application.StartupPath : templateFolder, "CG.fth.18");
			if (System.IO.File.Exists(templateHost))
				itemViewControl_.PreviewControl.Version = Svt.Caspar.Controls.FlashTemplateHostControl.Versions.Version18;
			else
			{
				//Try TemplateHost v1.7
				templateHost = System.IO.Path.Combine((string.IsNullOrEmpty(templateFolder)) ? Application.StartupPath : templateFolder, "CG.fth.17");
				if (System.IO.File.Exists(templateHost))
					itemViewControl_.PreviewControl.Version = Svt.Caspar.Controls.FlashTemplateHostControl.Versions.Version17;
				else
				{
					//Try TemplateHost v1.6
					templateHost = System.IO.Path.Combine((string.IsNullOrEmpty(templateFolder)) ? Application.StartupPath : templateFolder, "CG.fth");
					if (System.IO.File.Exists(templateHost))
						itemViewControl_.PreviewControl.Version = Svt.Caspar.Controls.FlashTemplateHostControl.Versions.Version16;
					else
						templateHost = string.Empty;
				}
			}
            
			if (!string.IsNullOrEmpty(templateHost))
			{
				itemViewControl_.PreviewControl.TemplateHost = templateHost;
                itemViewControl_.EnablePreview = Properties.Settings.Default.EnablePreview;
			}
			else
			{
				//TODO: Disable preview since there is no templatehost.
				itemViewControl_.EnablePreview = false;
			}
		}

		const string BaseRegistryKey = @"SOFTWARE\Sveriges Television AB\Caspar CG\";
		private string GetCasparTemplateFolder()
		{
			string casparTemplateFolder = string.Empty;
			Microsoft.Win32.RegistryKey baseKey = null;
			try
			{
				baseKey = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(BaseRegistryKey + "1.5");
				if (baseKey != null)
				{
					casparTemplateFolder = (string)baseKey.GetValue("TemplateFolder");
				}
			}
			catch { }
			finally
			{
				if (baseKey != null)
					baseKey.Close();
			}

			return casparTemplateFolder;
		}

		void lbRundown__Resize(object sender, EventArgs e)
		{
			lbRundown_.Invalidate();
		}

		#region Rundown menu
		private void UpdateRundownMenuState()
		{
			btnNew_.Enabled = !IsOnAir;
			btnDelete_.Enabled = (lbRundown_.SelectedIndex != ListBox.NoMatches) && !IsOnAir;

			btnLoad_.Enabled = btnPlay_.Enabled = btnUpdate_.Enabled = btnStop_.Enabled = (lbRundown_.SelectedIndex != ListBox.NoMatches && (HostsManager.ValidConnections != Hosts.ValidHostsConnections.None));
			btnClear_.Enabled = btnOnAir_.Enabled = (HostsManager.ValidConnections != Hosts.ValidHostsConnections.None);
		}

		private void btnNew__Click(object sender, EventArgs e)
		{
			InsertEmptyItem();
		}

		private void btnDelete__Click(object sender, EventArgs e)
		{
			DeleteSelectedItem();
		}

		private void btnLoad__Click(object sender, EventArgs e)
		{
			DoLoadItem((RundownItem)lbRundown_.SelectedItem, false);
		}

		private void btnPlay__Click(object sender, EventArgs e)
		{
			DoPlayItem((RundownItem)lbRundown_.SelectedItem);
		}

		private void btnStep__Click(object sender, EventArgs e)
		{
			DoStepItem((RundownItem)lbRundown_.SelectedItem);
		}

		private void btnStop__Click(object sender, EventArgs e)
		{
			RundownItem item = lbRundown_.SelectedItem as RundownItem;
            if (item != null)
            {
                if (item.IsCG)
                    DoStopItem(item);
                else
                    DoClear(item);
            }
		}

        private void btnUpdate__Click(object sender, EventArgs e)
        {
            DoUpdateItem((RundownItem)lbRundown_.SelectedItem);
        }
		private void btnClear__Click(object sender, EventArgs e)
		{
			DoClear((RundownItem)lbRundown_.SelectedItem);
		}
		#endregion

		#region On Air
		private void btnOnAir__CheckedChanged(object sender, EventArgs e)
		{
			if (btnOnAir_.Checked)
			{
				btnOnAir_.Text = "On Air";
				pOnAir_.BackColor = Color.LimeGreen;

				if (lbRundown_.SelectedItem != null)
					StartItemLoadTimer();
			}
			else
			{
				btnOnAir_.Text = "Off Air";
				pOnAir_.BackColor = Color.Transparent;
				
				StopItemLoadTimer();
				IsCurrentItemLoaded = false;
			}
			UpdateRundownMenuState();
			itemViewControl_.Enabled = (lbRundown_.SelectedItem != null && !IsOnAir);
		}
		public bool IsOnAir { get { return btnOnAir_.Checked; } }

		#endregion

		#region Rundownlist
		void lbRundown__DragDrop(object sender, DragEventArgs e)
		{
			IsDirty = true;
		}

		void itemViewControl__CurrentItemDataChanged(object sender, EventArgs e)
		{
			if (lbRundown_.SelectedIndex != ListBox.NoMatches)
			{
				lbRundown_.Invalidate(lbRundown_.GetItemRectangle(lbRundown_.SelectedIndex));
			}
			IsDirty = true;
		}

		void lbRundown__SelectedIndexChanged(object sender, EventArgs e)
		{
			if (sender == lbRundown_)
			{
				if (itemViewControl_.CurrentItem != null)
					itemViewControl_.CurrentItem.IsStarted = false;

				itemViewControl_.CurrentItem = (RundownItem)lbRundown_.SelectedItem;
				itemViewControl_.Enabled = (lbRundown_.SelectedItem != null && !IsOnAir);

				if (IsOnAir)
				{
					//if we move fast through the list, we need to stop the load-timer so that when we finaly stop on an item,
					//that item gets timer.Interval ms before it gets loaded.
					if (!IsCurrentItemLoaded)
						StopItemLoadTimer();

					IsCurrentItemLoaded = false;

					if (lbRundown_.SelectedItem != null)
					{
						RundownItem item = (RundownItem)lbRundown_.SelectedItem;
						StartItemLoadTimer();
					}
				}
				lbRundown_.Focus();

				UpdateRundownMenuState();
			}
		}
		
		private void InsertEmptyItem()
		{
			RundownItem item = new RundownItem();
            item.Color = itemViewControl_.TemplateColor;

			int originalSelection = lbRundown_.SelectedIndex;
			if (originalSelection == ListBox.NoMatches)
			{
				lbRundown_.Items.Add(item);
				lbRundown_.SelectedIndex = lbRundown_.Items.Count - 1;
			}
			else
			{
				//insert element over the currently selected item
				lbRundown_.Items.Insert(originalSelection + 1, item);
				lbRundown_.SelectedIndex = originalSelection + 1;
			}

			IsDirty = true;
		}

		private void DeleteSelectedItem()
		{
			int originalSelection = lbRundown_.SelectedIndex;
			if (originalSelection != ListBox.NoMatches)
			{
				lbRundown_.Items.RemoveAt(originalSelection);
				if (lbRundown_.Items.Count >= originalSelection + 1)
				{
					lbRundown_.SelectedIndex = originalSelection;
				}
				else if (originalSelection != 0)
					lbRundown_.SelectedIndex = originalSelection - 1;
				else
					lbRundown_.SelectedIndex = ListBox.NoMatches;
			}
		}
		#endregion

		#region main form events
		private void Form1_Load(object sender, EventArgs e)
		{
			Application.AddMessageFilter(this);
			UpdateHosts(Properties.Settings.Default.Channels);
			HostsManager.Connect();
			HostsManager.UpdatedMediafiles += new EventHandler<Caspar_Pilot.Hosts.HostEventArgs>(HostsManager_UpdatedMediafiles);
			HostsManager.UpdatedTemplates += new EventHandler<Caspar_Pilot.Hosts.HostEventArgs>(HostsManager_UpdatedTemplates);
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (CheckSaveBefore() != DialogResult.Cancel)
			{
				HostsManager.Disconnect();

				Properties.Settings.Default.TemplateFolder = itemViewControl_.PreviewControl.TemplateFolder;
				
                Properties.Settings.Default.TemplateColor = itemViewControl_.TemplateColor.Name;
                Properties.Settings.Default.GraphicColor = itemViewControl_.GraphicColor.Name;
                Properties.Settings.Default.MultistepColor = itemViewControl_.MultistepColor.Name;

                Properties.Settings.Default.LoadKey = hotKeys_["Load"].ToString();
                Properties.Settings.Default.PlayKey = hotKeys_["Play"].ToString();
                Properties.Settings.Default.StepKey = hotKeys_["Step"].ToString();
                Properties.Settings.Default.StopKey = hotKeys_["Stop"].ToString();
                Properties.Settings.Default.UpdateKey = hotKeys_["Update"].ToString();
				Properties.Settings.Default.QuickPlayKey = hotKeys_["QuickPlay"].ToString();
				Properties.Settings.Default.QuickStopKey = hotKeys_["QuickStop"].ToString();
				Properties.Settings.Default.QuickStepKey = hotKeys_["QuickStep"].ToString();

                Properties.Settings.Default.Save();
			}
			else
				e.Cancel = true;
		}
		#endregion

		#region Rundown ownerdraw
		void lbRundown__DrawItem(object sender, DrawItemEventArgs e)
		{
			if (e.Index >= 0)
			{
				RundownItem item = (RundownItem)lbRundown_.Items[e.Index];

				bool bSelected = ((e.State & DrawItemState.Selected) == DrawItemState.Selected);
				Color backColor = bSelected ? Color.Black : item.Color;
				Brush fontBrush = bSelected ? fontBrushSelected_ : fontBrush_;
				e.Graphics.FillRectangle(new SolidBrush(backColor), e.Bounds);

                int offsetX = 18;
                if (item.IsControl)
                {
                    e.Graphics.FillRectangle(new SolidBrush(Color.White), e.Bounds.Right - 17, e.Bounds.Top, 17, e.Bounds.Height);
                }
                else
                {
                    //Show warning when a channel is selected that is not availible   
                    Hosts.ChannelInformation channel = HostsManager.GetChannelInfo(item.Channel);
                    Color channelColor = (channel != null) ? Color.FromArgb(channel.ArgbValue) : Color.Red;
                    e.Graphics.FillRectangle(new SolidBrush(channelColor), e.Bounds.Right - 17, e.Bounds.Top, 17, e.Bounds.Height);
                    if (channel == null || channel.Online == false)
                        e.Graphics.DrawImage(Properties.Resources.warn, e.Bounds.Right - 16, e.Bounds.Bottom - 16, 14, 14);
                }

                e.Graphics.DrawString(item.Channel, e.Font, fontBrush, e.Bounds.Right - offsetX, e.Bounds.Bottom - 17, channelStringFormat_);

                string videoLayer = (item.IsCG == true) ? item.CGItem.VideoLayer.ToString() : item.StaticItem.VideoLayer.ToString();
                e.Graphics.DrawString(string.Format("Layer: {0}", videoLayer), e.Font, fontBrush, e.Bounds.Right - offsetX, e.Bounds.Y + 2, channelStringFormat_);
				
				if (item.IsCG)
					DrawCGItem(item, e, bSelected);
				else
					DrawStaticItem(item, e, bSelected);

				e.Graphics.DrawLine(Pens.Black, e.Bounds.Left, e.Bounds.Bottom - 1, e.Bounds.Right, e.Bounds.Bottom - 1);
			}
			else
			{
				e.DrawBackground();                
			}

			//e.DrawFocusRectangle();
		}

		private void DrawCGItem(RundownItem item, System.Windows.Forms.DrawItemEventArgs e, bool bSelected)
		{
			Brush fontBrush = bSelected ? fontBrushSelected_ : fontBrush_;
            string title = string.IsNullOrEmpty(item.Page) ? string.Empty : (item.Page + ": ");
			title += string.IsNullOrEmpty(item.Name) ? NewItemName : item.Name;
			e.Graphics.DrawString(title, e.Font, fontBrush, e.Bounds.X + 2, e.Bounds.Y + 2);

            if (!item.IsDataUpdate)
            {
                //Visa varnings-ikon om ingen mall är vald
                int offsetX = 0;
                if (string.IsNullOrEmpty(item.CGItem.TemplateIdentifier))
                {
                    e.Graphics.DrawImage(Properties.Resources.warn, e.Bounds.X + 2, e.Bounds.Bottom - 16, 14, 14);
                    offsetX = 18;
                }

                //visa vilken still det handlar om
                e.Graphics.DrawString("Mall: " + item.CGItem.TemplateIdentifier, e.Font, fontBrush, e.Bounds.X + 2 + offsetX, e.Bounds.Bottom - 17);
            }
            else
            {
                e.Graphics.DrawString("JUST DATA UPDATE", e.Font, fontBrush, e.Bounds.X + 2, e.Bounds.Bottom - 17);
            }
		}

		private void DrawStaticItem(RundownItem item, System.Windows.Forms.DrawItemEventArgs e, bool bSelected)
		{
			Brush fontBrush = bSelected ? fontBrushSelected_ : fontBrush_;
            string title = string.IsNullOrEmpty(item.Page) ? string.Empty : (item.Page + ": ");

            if (item.IsControl)
            {
                title += string.IsNullOrEmpty(item.Name) ? NewItemName : item.Name;
            }
            else
            {
                title += string.IsNullOrEmpty(item.Name) ? NewItemName : item.Name + (" (" + item.StaticItem.Transition.Type.ToString());
                if (item.StaticItem.Transition.Duration != 0)
                    title += ", " + item.StaticItem.Transition.Duration + " frames";

                title += ") " + (item.StaticItem.Loop ? " LOOP" : "");
            }

			e.Graphics.DrawString(title, e.Font, fontBrush, e.Bounds.X + 2, e.Bounds.Y + 2);

			//Visa varnings-ikon om ingen still är vald
			int offsetX = 0;
			if (string.IsNullOrEmpty(item.StaticItem.Clipname))
			{
				e.Graphics.DrawImage(Properties.Resources.warn, e.Bounds.X + 2, e.Bounds.Bottom - 16, 14, 14);
				offsetX = 18;
			}

			//visa vilken still det handlar om
            if (item.IsControl)
                e.Graphics.DrawString("Setting: " + item.StaticItem.Clipname, e.Font, fontBrush, e.Bounds.X + 2 + offsetX, e.Bounds.Bottom - 17);
            else
                e.Graphics.DrawString("Fil: " + item.StaticItem.Clipname, e.Font, fontBrush, e.Bounds.X + 2 + offsetX, e.Bounds.Bottom - 17);
		}

		#endregion

		#region Serialization helpers
		string currentFilename_;
		string CurrentFilename
		{
			get
			{
				return currentFilename_;
			}
			set
			{
				currentFilename_ = value;
				UpdateTitle();
			}
		}

		bool bIsDirty_ = false;
		bool IsDirty 
		{
			get { return bIsDirty_; }
			set
			{
				if (value != bIsDirty_)
				{
					bIsDirty_ = value;
					UpdateTitle();
				}
			} 
		}
		DialogResult CheckSaveBefore()
		{
			if (IsDirty)
			{
				DialogResult result = MessageBox.Show("Do you want to save?", "Confirm", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
				if (result == DialogResult.Yes)
					miSave__Click(this, EventArgs.Empty);

				return result;
			}
			else
				return DialogResult.No;
		}
		private void UpdateTitle()
		{
			this.Text = "CasparCG Client " + VersionString;
			if (IsDirty)
				this.Text += " *";
			if (!string.IsNullOrEmpty(currentFilename_))
				this.Text += " - " + currentFilename_;
		}
		#endregion

		#region main menu
		private void miNew__Click(object sender, EventArgs e)
		{
			if (CheckSaveBefore() != DialogResult.Cancel)
			{
				lbRundown_.Items.Clear();
				IsDirty = false;
			}
		}

		private void miLoad__Click(object sender, EventArgs e)
		{
			if (CheckSaveBefore() != DialogResult.Cancel)
			{
				OpenFileDialog ofd = new OpenFileDialog();
				ofd.Multiselect = false;
				ofd.Filter = "Caspar rundown (*.crx)|*.crx";
				if (ofd.ShowDialog() == DialogResult.OK)
					if (LoadFile(ofd.FileName))
						CurrentFilename = ofd.FileName;
			}
		}

		private void miSave__Click(object sender, EventArgs e)
		{
			if (!string.IsNullOrEmpty(CurrentFilename))
				SaveFile(CurrentFilename);
			else
				miSaveAs__Click(sender, e);
		}

		private void miSaveAs__Click(object sender, EventArgs e)
		{
			SaveFileDialog sfd = new SaveFileDialog();
			sfd.Filter = "Caspar rundown (*.crx)|*.crx";
			if (sfd.ShowDialog() == DialogResult.OK)
			{
				if (SaveFile(sfd.FileName))
					CurrentFilename = sfd.FileName;
			}
		}

		private void miSettings__Click(object sender, EventArgs e)
		{
			SettingsDialog sd = new SettingsDialog();
			sd.LocalTemplateFolder = itemViewControl_.PreviewControl.TemplateFolder;
           
            sd.TemplateColor = itemViewControl_.TemplateColor;
            sd.GraphicColor = itemViewControl_.GraphicColor; 
            sd.MultistepColor = itemViewControl_.MultistepColor;
            sd.HotKeys = hotKeys_;

			sd.DefaultFieldPrefix = Properties.Settings.Default.DefaultFieldNamePrefix;
			sd.DefaultFieldStartIndex = Properties.Settings.Default.DefaultFieldNameStartIndex;
			sd.Channels = Properties.Settings.Default.Channels;
			sd.PreviewBackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;

			if (sd.ShowDialog() == DialogResult.OK)
			{
				Properties.Settings.Default.TemplateFolder = sd.LocalTemplateFolder;
				Properties.Settings.Default.PreviewBackgroundColor = sd.PreviewBackgroundColor;
				itemViewControl_.PreviewControl.BackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;
				SetPreviewControlPaths();
                
                itemViewControl_.TemplateColor  = sd.TemplateColor;
                itemViewControl_.GraphicColor   = sd.GraphicColor;
                itemViewControl_.MultistepColor = sd.MultistepColor;

                hotKeys_ = sd.HotKeys;

				Properties.Settings.Default.DefaultFieldNamePrefix = sd.DefaultFieldPrefix;
				Properties.Settings.Default.DefaultFieldNameStartIndex = sd.DefaultFieldStartIndex;
				Properties.Settings.Default.Channels = sd.Channels;

				Properties.Settings.Default.Save();

				UpdateHosts(sd.Channels);
				lbRundown_.Refresh();
			}
		}

		private void miExit__Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void miCasparStatus__DoubleClick(object sender, EventArgs e)
		{
			HostsManager.Connect();
		}

		private void miArkiv__DropDownOpening(object sender, EventArgs e)
		{
			miLoad_.Enabled = miNew_.Enabled = miSettings_.Enabled = !IsOnAir;
		}

		private void btnRefreshTemplates__Click(object sender, EventArgs e)
		{
			if (lbRundown_.SelectedItem != null)
			{
				RundownItem item = (RundownItem)lbRundown_.SelectedItem;
				Hosts.ChannelInformation channel = HostsManager.GetChannelInfo(item.Channel);
				if (channel != null)
				{
					Hosts.DeviceHolder device = HostsManager.GetDevice(channel.Hostname);
					if (device != null)
					{
						device.RefreshTemplates();
					}
				}
			}
		}

		private void btnRefreshGraphics__Click(object sender, EventArgs e)
		{
			if (lbRundown_.SelectedItem != null)
			{
				RundownItem item = (RundownItem)lbRundown_.SelectedItem;
				Hosts.ChannelInformation channel = HostsManager.GetChannelInfo(item.Channel);
				if (channel != null)
				{
					Hosts.DeviceHolder device = HostsManager.GetDevice(channel.Hostname);
					if (device != null)
					{
						device.RefreshMediafiles();
					}
				}
			}
		}

		private void tankaMallgrafiklistaToolStripMenuItem_Click(object sender, EventArgs e)
		{
			itemViewControl_.LoadTemplatesFromOfflinefolder();
		}
		#endregion

		#region Serialize
		bool LoadFile(string filename)
		{
			bool returnvalue = true;
			try
			{
				List<RundownItem> newItems = new List<RundownItem>();

				XPathDocument xpDoc = new XPathDocument(filename);
				XPathNavigator xpNav = xpDoc.CreateNavigator();
				XPathNodeIterator items = xpNav.Select("/rundown/rundownitem");
				while (items.MoveNext())
				{
					try
					{
						RundownItem item = new RundownItem();
						item.Name = (string)items.Current.Evaluate("string(@name)");
						item.Channel = (string)items.Current.Evaluate("string(@channel)");
                        item.Page = (string)items.Current.Evaluate("string(@page)");

                        string controlvalue = items.Current.Evaluate("string(@controlvalue)").ToString();
                        if (!string.IsNullOrEmpty(controlvalue))
                            item.ControlValue = int.Parse(controlvalue);

                        string controlsystem = items.Current.Evaluate("string(@controlsystem)").ToString();
                        if (!string.IsNullOrEmpty(controlsystem))
                            item.Channel = controlsystem;

                        string controlpreset = items.Current.Evaluate("string(@controlpreset)").ToString();
                        if (!string.IsNullOrEmpty(controlpreset))
                            item.StaticItem.Clipname = controlpreset;

						string strMultiStep = (string)items.Current.Evaluate("string(@multistep)");
						bool bMultiStep = false;
						Boolean.TryParse(strMultiStep, out bMultiStep);
						item.MultiStep = bMultiStep;

						string strIsStoreData = (string)items.Current.Evaluate("string(@isstoredata)");
						bool bIsStoreData = false;
						Boolean.TryParse(strIsStoreData, out bIsStoreData);
						item.IsStoredData = bIsStoreData;

                        string strColor = items.Current.Evaluate("string(@color)") as String;
                        item.Color = Color.FromName(strColor);

                        string strIsControl = (string)items.Current.Evaluate("string(@iscontrol)");
                        bool bIsControl = false;
                        Boolean.TryParse(strIsControl, out bIsControl);
                        item.IsControl = bIsControl;

						items.Current.MoveToFirstChild();
						if (items.Current.Name == "cgitem")
						{
							item.IsCG = true;
							item.CGItem.ReadXml(items.Current.ReadSubtree());
						}
						else if (items.Current.Name == "item")
						{
							item.IsCG = false;
							item.StaticItem = Svt.Caspar.CasparItem.Create(items.Current.ReadSubtree());
						}

						newItems.Add(item);
					}
					catch { }
				}

				{
					lbRundown_.SuspendLayout();
					lbRundown_.Items.Clear();
					foreach (RundownItem item in newItems)
						lbRundown_.Items.Add(item);
					lbRundown_.ResumeLayout();

				}
			}
			catch (Exception ex)
			{
				returnvalue = false;
				MessageBox.Show(ex.Message, "There was an error when loading rundown");
			}

			IsDirty = !returnvalue;
			return returnvalue;
		}

		bool SaveFile(string filename)
		{
			bool returnvalue = true;
			try
			{
				XmlWriterSettings settings = new XmlWriterSettings();
				settings.Indent = true;
				using (XmlWriter writer = XmlWriter.Create(filename, settings))
				{
					writer.WriteStartElement("rundown");
					foreach (RundownItem item in lbRundown_.Items)
					{
						writer.WriteStartElement("rundownitem");
						writer.WriteAttributeString("name", item.Name);
						writer.WriteAttributeString("multistep", item.MultiStep.ToString());
						writer.WriteAttributeString("isstoredata", item.IsStoredData.ToString());
                        writer.WriteAttributeString("color", item.Color.Name);
						writer.WriteAttributeString("channel", item.Channel);
                        writer.WriteAttributeString("page", item.Page);
                        writer.WriteAttributeString("iscontrol", item.IsControl.ToString());
                        writer.WriteAttributeString("controlvalue", item.ControlValue.ToString());
                        writer.WriteAttributeString("controlsystem", item.Channel.ToString());
                        writer.WriteAttributeString("controlpreset", item.StaticItem.Clipname.ToString());

						if (item.IsCG)
							item.CGItem.WriteXml(writer);
						else
							item.StaticItem.WriteXml(writer);

						writer.WriteEndElement();
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "There was an error when the saving the file");
				returnvalue = false;
			}

			IsDirty = !returnvalue;
			return returnvalue;
		}
		#endregion

		#region playout
		bool currentItemLoaded_ = false;
		bool IsCurrentItemLoaded 
		{
			get { return currentItemLoaded_; }
			set
			{
				currentItemLoaded_ = value;
				//pItemLoaded_.BackColor = (value) ? Color.Orange : Color.Transparent;
			}
		}

		void StartItemLoadTimer()
		{
			itemLoadTimer_.Start();
		}
		void StopItemLoadTimer()
		{
			itemLoadTimer_.Stop();
		}

		private void itemLoadTimer__Tick(object sender, EventArgs e)
		{
			itemLoadTimer_.Stop();

			if (IsOnAir)
			{
				DoLoadItem((RundownItem)lbRundown_.SelectedItem, false);
				if(lbRundown_.SelectedItem != null)
					IsCurrentItemLoaded = true;
			}
		}

		void DoPlayCurrentItem()
		{
            RundownItem item = (RundownItem)lbRundown_.SelectedItem;
            bool IsUpdate = (item != null) ? item.IsDataUpdate : false;

			if (!IsCurrentItemLoaded && !IsUpdate)
			{
				StopItemLoadTimer();
				DoLoadItem((RundownItem)lbRundown_.SelectedItem, true);
			}
			else
				DoPlayItem((RundownItem)lbRundown_.SelectedItem);
		}

		void DoStepItem(RundownItem item)
		{
			if (item != null)
			{
				if (item.IsCG)
				{
					Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
					if (channel != null)
						channel.CG.Next(item.CGItem.VideoLayer, item.CGItem.Layer);
				}
			}
		}

		void DoPlayItem(RundownItem item)
		{
			if (item != null)
			{
				Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
				if (item.IsControl)
                {
                    SpyderClient.Send(item);
                }
                else if (item.IsCG)
				{
                    if (channel != null)
                    {
                        if(item.IsDataUpdate)
                            channel.CG.Update(item.CGItem.VideoLayer, item.CGItem.Layer, new CGDataListWrapper(item.CGItem.Data));
                        else
                            channel.CG.Play(item.CGItem.VideoLayer, item.CGItem.Layer);
                    }
				}
				else
				{
					if (channel != null)
						channel.Play(item.StaticItem.VideoLayer);
				}
			}
		}

        void DoUpdateItem(RundownItem item)
        {
            if (item != null)
            {
                Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
                if (item.IsCG)
                {
                    if (channel != null)
                        channel.CG.Update(item.CGItem.VideoLayer, item.CGItem.Layer, new CGDataListWrapper(item.CGItem.Data));
                }
            }
        }

		void DoLoadItem(RundownItem item, bool bAutoPlay)
		{
			if(item != null)
			{
				Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
                if (item.IsControl && bAutoPlay)
                {
                    SpyderClient.Send(item);
                }
                else if (item.IsCG)
                {
                    if (channel != null)
                    {
                        if (item.IsStoredData)
                            channel.CG.Add(item.CGItem.VideoLayer, item.CGItem.Layer, item.CGItem.TemplateName, bAutoPlay, item.CGItem.Data[0].Value);
                        else
                            channel.CG.Add(item.CGItem, bAutoPlay);
                    }
                }
                else
                {
                    if (channel != null)
                    {
                        channel.LoadBG(item.StaticItem);
                        if (bAutoPlay)
                            channel.Play(item.StaticItem.VideoLayer);
                    }
                }
			}
		}
		void DoStopItem(RundownItem item)
		{
			if (item != null)
			{
				Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
				if (item.IsCG)
				{
					if (channel != null)
						channel.CG.Stop(item.CGItem.VideoLayer, item.CGItem.Layer);
				}
				else
				{
					if (channel != null)
						channel.Stop(item.StaticItem.VideoLayer);
				}
			}
		}
			        
		void DoClear(RundownItem item)
		{
			if (item != null)
			{
				Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
				if (channel != null)
				{
					if (item.IsCG)
					{
						channel.CG.Clear(item.CGItem.VideoLayer);
					}
					else
					{
						Svt.Caspar.CasparItem blackItem = new Svt.Caspar.CasparItem("#00000000");
                        blackItem.VideoLayer = item.StaticItem.VideoLayer;
						channel.Load(blackItem);
					}
				}
			}
		}
		#endregion

		#region IMessageFilter Members
		public bool PreFilterMessage(ref Message m)
		{
			//hotkeys
            if (m.Msg == Messages.WM_KEYDOWN)
            {
                int key = m.WParam.ToInt32();

	 			if (key == (int)Keys.PageDown || key == (int)Keys.End) // PageDown = Spela två rader. End = Stoppa två rader.
				{
					if (key == 34)
					{
						DoPlayCurrentItem();
						if (lbRundown_.Items.Count > lbRundown_.SelectedIndex + 1)
						{
							lbRundown_.SelectedIndex++;
							DoPlayCurrentItem();
							lbRundown_.SelectedIndex--;
						}
					}
					else
					{
						int index1 = lbRundown_.SelectedIndex;
						int index2 = lbRundown_.SelectedIndex + 1;
						RundownItem item1 = lbRundown_.Items[index1] as RundownItem;
						RundownItem item2 = lbRundown_.Items[index2] as RundownItem;
						
						if (item1.IsCG)
							DoStopItem(item1);
						else
							DoClear(item1);

						if (item2.IsCG)
							DoStopItem(item2);
						else
							DoClear(item2);
					}

					return true;
				}
				else if (key == (int)Keys.Pause)	// Pause = Clear.
				{
					for (int i = 0; i < lbRundown_.Items.Count; i++)
					{
						DoClear(lbRundown_.Items[i] as RundownItem);
					}
				}
				else if (hotKeys_.ContainsKey("QuickPlay") && (int)hotKeys_["QuickPlay"] == key)
					DoPlayCurrentItem();
				else if (hotKeys_.ContainsKey("QuickStop") && (int)hotKeys_["QuickStop"] == key)
					btnStop__Click(null, null);
				else if (hotKeys_.ContainsKey("QuickStep") && (int)hotKeys_["QuickStep"] == key)
					btnStep__Click(null, null);
                else if (hotKeys_.ContainsKey("Update") && (int)hotKeys_["Update"] == key)
                    btnUpdate__Click(null, null);

				else if (IsOnAir)
				{
					if ((int)hotKeys_["Load"] == key)
						btnLoad__Click(null, null);
					else if ((int)hotKeys_["Play"] == key)
						btnPlay__Click(null, null);
					else if ((int)hotKeys_["Step"] == key)
						btnStep__Click(null, null);
					else if ((int)hotKeys_["Stop"] == key)
						btnStop__Click(null, null);
                    else if ((int)hotKeys_["Update"] == key)
                        btnUpdate__Click(null, null);
				}
            }

			if (IsOnAir)
			{
				if (m.Msg == Messages.WM_KEYDOWN)
				{                   
					int key = m.WParam.ToInt32(); 

                    if (key == (int)Keys.Space)
					{
						RundownItem item = (RundownItem)lbRundown_.SelectedItem;
						if (item != null)
						{
							bool bDoAdvance = true;
							if (item.IsCG && item.MultiStep)
							{
								bDoAdvance = false;
								if (item.IsStarted)
									DoStepItem(item);
								else
								{
									DoPlayCurrentItem();
									item.IsStarted = true;
								}
							}
							else 
								DoPlayCurrentItem();

							if (bDoAdvance)
							{
								int nextIndex = lbRundown_.SelectedIndex + 1;
								if (nextIndex < lbRundown_.Items.Count)
									lbRundown_.SelectedIndex = nextIndex;
								else
									lbRundown_.ClearSelected();
							}
						}
					}
				}
			}
			return false;
		}
		#endregion

		void HostsManager_UpdatedTemplates(object sender, Hosts.HostEventArgs e)
		{
			itemViewControl_.UpdateTemplates();
		}

		void HostsManager_UpdatedMediafiles(object sender, Hosts.HostEventArgs e)
		{
			itemViewControl_.UpdateMediafiles();
		}

        #region Host management
        /// <summary>
		/// Called when the app starts or when the user changes settings
		/// </summary>
		/// <param name="channels"></param>
		private void UpdateHosts(Caspar_Pilot.Hosts.ChannelInformationList channels)
        {
            IEnumerable<Hosts.DeviceHolder> devices = null;

            //unsubscribe validConnectionChanged-events
            {
                devices = HostsManager.Devices;
                if (devices != null)
                    foreach (Hosts.DeviceHolder device in devices)
                        device.VaildConnectionChanged -= device_VaildConnectionChanged;
            }

            HostsManager.SetChannels(channels);

            //Rebuild caspar host status GUI-elements
            miCasparStatus_.DropDownItems.Clear();
            devices = HostsManager.Devices;
            if (devices != null)
            {
                foreach (Hosts.DeviceHolder device in devices)
                {
                    ToolStripMenuItem item = new ToolStripMenuItem();
                    item.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
                    item.Image = device.HasValidConnection ? Properties.Resources.connected : Properties.Resources.disconnected;
                    item.Name = device.Hostname;
                    item.Size = new System.Drawing.Size(100, 20);
                    item.Text = device.Hostname;
                    item.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
                    item.Click += new System.EventHandler(this.miDeviceStatus__Click);
                    miCasparStatus_.DropDownItems.Add(item);

                    //subscribe to validConnectionChanged-events
                    device.VaildConnectionChanged += device_VaildConnectionChanged;
                }
            }

            itemViewControl_.SetChannels(channels);

            lbRundown_.Invalidate();
        }

		void device_VaildConnectionChanged(object sender, EventArgs e)
		{
            if (InvokeRequired)
            {
                BeginInvoke(new EventHandler<EventArgs>(device_VaildConnectionChanged), sender, e);
            }
            else
            {
                try
                {
                    Hosts.DeviceHolder device = (Hosts.DeviceHolder)sender;

                    ToolStripItem[] items = miCasparStatus_.DropDownItems.Find(device.Hostname, false);
                    foreach (ToolStripItem item in items)
                        item.Image = device.HasValidConnection ? Properties.Resources.connected : Properties.Resources.disconnected;

                    //invalidate affecetd items in the rundown
                    int startIndex = lbRundown_.IndexFromPoint(0, 0);
                    int lastIndex = Math.Min(startIndex + lbRundown_.Height / lbRundown_.ItemHeight, lbRundown_.Items.Count - 1);
                    foreach (Hosts.ChannelInformation channel in device.Channels)
                    {
                        for (int index = startIndex; index < lastIndex; ++index)
                        {
                            RundownItem item = (RundownItem)lbRundown_.Items[index];
                            if (item.Channel == channel.Label)
                                lbRundown_.Invalidate(lbRundown_.GetItemRectangle(index), false);
                        }
                    }
                }
                catch { }
            }
		}

        private void miDeviceStatus__Click(object sender, EventArgs e)
        {
            //ToolStripMenuItem item = (ToolStripMenuItem)sender;
            //Hosts.DeviceHolder device = HostsManager.GetDevice(item.Name);
        }

        void HostsManager_ValidConnectionsChanged(object sender, EventArgs e)
        {
            if (InvokeRequired)
                BeginInvoke(new EventHandler<EventArgs>(HostsManager_ValidConnectionsChanged), sender, e);
            else
            {
                UpdateRundownMenuState();

                switch (HostsManager.ValidConnections)
                {
                    case Caspar_Pilot.Hosts.ValidHostsConnections.All:
                        miCasparStatus_.Image = Properties.Resources.connected;
                        break;

                    case Caspar_Pilot.Hosts.ValidHostsConnections.Some:
                        miCasparStatus_.Image = Properties.Resources.semi_connected;
                        break;

                    case Caspar_Pilot.Hosts.ValidHostsConnections.None:
                    default:
                        miCasparStatus_.Image = Properties.Resources.disconnected;
                        break;
                }
            }
        }
        #endregion

        private void btnDetail__Click(object sender, EventArgs e)
        {
            if(btnDetail_.Checked == true)
                lbRundown_.ItemHeight = 64;
            else
                lbRundown_.ItemHeight = 32;
        }

        private void toggleFullscreenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized)
            {
                this.WindowState = FormWindowState.Normal;
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
                this.TopMost = false;
                
            }
            else if (this.WindowState == FormWindowState.Normal)
            {  
                this.WindowState = FormWindowState.Maximized;
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                Interop.SetWinFullScreen(this.Handle);
                this.TopMost = true; 
            }
        }

        private void togglePreviewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.itemViewControl_.EnablePreview = !this.itemViewControl_.EnablePreview;
        }

        private void OperatorForm_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!btnOnAir_.Checked && !lbRundown_.Focused)
            {
                this.Page = null;
                return;
            }

            if (e.KeyChar == '\r')
            {
                this.IsEnterPressed = true;
                for (int i = 0; i < lbRundown_.Items.Count; i++)
                {
                    RundownItem item = lbRundown_.Items[i] as RundownItem;
                    if (item.Page == this.Page)
                    {
                        lbRundown_.SetSelected(i, true);
                        DoPlayCurrentItem();
                        break;
                    }
                }
            }
            else if ((e.KeyChar >= 48 && e.KeyChar <= 57))
            {
                if (!this.IsEnterPressed)
                {
                    this.Page += new KeysConverter().ConvertToString(e.KeyChar);
                }
                else
                {
                    this.Page = new KeysConverter().ConvertToString(e.KeyChar);
                    this.IsEnterPressed = false;
                }
            }
            else if (e.KeyChar == 43)
            {
                this.Page = (Convert.ToInt32(this.Page) + 1).ToString();
                DoPlayCurrentPageItem();
            }
            else if (e.KeyChar == 45)
            {
                this.Page = (Convert.ToInt32(this.Page) - 1).ToString();
                DoPlayCurrentPageItem();
            }

            this.toolStripStatusLabelPage.Text = this.Page;
        }

        private void DoPlayCurrentPageItem()
        {
            for (int i = 0; i < lbRundown_.Items.Count; i++)
            {
                RundownItem item = lbRundown_.Items[i] as RundownItem;
                if (item.Page == this.Page)
                {
                    lbRundown_.SetSelected(i, true);
                    DoPlayCurrentItem();
                    break;
                }
            }
        }

        private void compactModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.IsCompactMode)
            {
                this.Width = this.windowWidth;
                this.IsCompactMode = false;
            }
            else
            {
                this.windowWidth = this.Width;
                this.Width = 388;
                this.IsCompactMode = true;
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutDialog dialog = new AboutDialog();
            dialog.ShowDialog();
        }

        private void templateDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.itemViewControl_.EnableTemplateData = !this.itemViewControl_.EnableTemplateData;
        }

        private void outputSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.itemViewControl_.EnableOutputSettings = !this.itemViewControl_.EnableOutputSettings;
        }

	}
}
