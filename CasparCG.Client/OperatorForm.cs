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
	public partial class OperatorForm : Form
	{
        const string RundownItemDataFormatString = "RundownItem";
        DataFormats.Format RundownItemDataFormat = null;

        public string VersionString = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).FileVersion;

		private static string NewItemName = "[New event]";
		private delegate void UpdateGUI(object parameter);

		internal Hosts.HostsManager HostsManager { get; private set; }

		public delegate void StringDelegate(string name);

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
            RundownItemDataFormat = DataFormats.GetFormat(RundownItemDataFormatString);

            HostsManager = new Hosts.HostsManager();
            HostsManager.ValidConnectionsChanged += HostsManager_ValidConnectionsChanged;

            channelStringFormat_ = new StringFormat(StringFormatFlags.NoWrap);
            channelStringFormat_.Alignment = StringAlignment.Far;
            fontBrush_ = new SolidBrush(Color.Black);
            fontBrushSelected_ = new SolidBrush(Color.White);

            InitializeComponent();
            for (int i = 0; i < 8; ++i)
            {
                miRecentFiles_.DropDownItems.Insert(0, new ToolStripMenuItem(string.Empty, null, recentFile__Click));
            }
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
                lbRundown_.DragDropDataFormat = RundownItemDataFormatString;
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
				gbRundown_.Controls.Add(lbRundown_);
                gbRundown_.Controls.Add(itemsMenu);
			}

			//Right side: View the selected item
			{
				itemViewControl_ = new Caspar_Pilot.Controls.ItemViewControl();
				itemViewControl_.OperatorForm = this;
				itemViewControl_.Enabled = false;
				itemViewControl_.Dock = DockStyle.Fill;
				itemViewControl_.CurrentItemDataChanged += itemViewControl__CurrentItemDataChanged;
                itemViewControl_.CurrentItemChannelChanged += itemViewControl__CurrentItemChannelChanged;
				itemViewControl_.PreviewControl.BackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;
				SetPreviewControlPaths();

                this.panel1.Controls.Add(itemViewControl_);
				//this.tableLayoutPanel1.Controls.Add(itemViewControl_, 2, 0);
			}

            this.itemViewControl_.EnableOutputSettings = Properties.Settings.Default.EnableOutputSettings;
            this.itemViewControl_.EnableTemplateData = Properties.Settings.Default.EnableTemplateData;
            this.itemViewControl_.EnableVideoLayers = Properties.Settings.Default.EnableVideoLayers;
            this.itemViewControl_.EnableControlValues = Properties.Settings.Default.EnableControlValues;
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
            RundownItem selectedItem = (RundownItem)lbRundown_.SelectedItem;

			btnNew_.Enabled = !IsOnAir;
            btnDelete_.Enabled = miCut_.Enabled = miCopy_.Enabled = (selectedItem != null) && !IsOnAir;
            miAdvance_.Enabled = IsOnAir && (selectedItem != null);

            btnClear_.Enabled = btnLoad_.Enabled = btnPlay_.Enabled = btnStep_.Enabled = btnUpdate_.Enabled = btnStop_.Enabled = 
            miClearLayer_.Enabled = miClearChannel_.Enabled = miLoad_.Enabled = miPlay_.Enabled = miStep_.Enabled = miUpdate_.Enabled = miStop_.Enabled = (selectedItem != null && selectedItem.Online);

            miClearAll_.Enabled = btnOnAir_.Enabled = (HostsManager.ValidConnections != Hosts.ValidHostsConnections.None);
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
            miLoad_.PerformClick();
		}

		private void btnPlay__Click(object sender, EventArgs e)
		{
            miPlay_.PerformClick();
        }

		private void btnStep__Click(object sender, EventArgs e)
		{
            miStep_.PerformClick();
		}

		private void btnStop__Click(object sender, EventArgs e)
		{
            miStop_.PerformClick();
		}

        private void btnUpdate__Click(object sender, EventArgs e)
        {
            miUpdate_.PerformClick();
        }
		private void btnClear__Click(object sender, EventArgs e)
		{
            if (btnClear_.Tag == btnClearLayer_.Tag)
                btnClearLayer_.PerformClick();
            else if (btnClear_.Tag == btnClearChannel_.Tag)
                btnClearChannel_.PerformClick();
		}
        private void btnClearLayer__Click(object sender, EventArgs e)
        {
            miClearLayer_.PerformClick();
            btnClear_.Tag = btnClearLayer_.Tag;
            btnClear_.Text = btnClearLayer_.Text;
        }
        private void btnClearChannel__Click(object sender, EventArgs e)
        {
            miClearChannel_.PerformClick();
            btnClear_.Tag = btnClearChannel_.Tag;
            btnClear_.Text = btnClearChannel_.Text;
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

        void itemViewControl__CurrentItemChannelChanged(object sender, EventArgs e)
        {
            RundownItem item = (RundownItem)lbRundown_.SelectedItem;
            if (item != null)
            {
                Hosts.ChannelInformation channel = HostsManager.GetChannelInfo(item.Channel);
                item.ChannelColor = (channel != null) ? Color.FromArgb(channel.ArgbValue) : Color.Red;
                item.Online = (channel != null) ? channel.Online : false;
                UpdateRundownMenuState();
            }
        }

		void lbRundown__SelectedIndexChanged(object sender, EventArgs e)
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
					StartItemLoadTimer();
			}
			lbRundown_.Focus();

			UpdateRundownMenuState();
		}

        void InsertEmptyItem()
        {
            RundownItem item = new RundownItem();
            item.Color = itemViewControl_.TemplateColor;
            InsertItem(item);
        }
		
		private void InsertItem(RundownItem item)
		{
            if (item != null)
            {
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

                IsDirty = true;
			}
		}
		#endregion

		#region main form events
		private void Form1_Load(object sender, EventArgs e)
		{
            if (Properties.Settings.Default.RecentFiles == null)
                Properties.Settings.Default.RecentFiles = new System.Collections.Specialized.StringCollection();
            
            UpdateHosts(Properties.Settings.Default.Channels);

			HostsManager.UpdatedMediafiles += new EventHandler<Caspar_Pilot.Hosts.HostEventArgs>(HostsManager_UpdatedMediafiles);
			HostsManager.UpdatedTemplates += new EventHandler<Caspar_Pilot.Hosts.HostEventArgs>(HostsManager_UpdatedTemplates);
			HostsManager.Connect();
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (CheckSaveBefore() != DialogResult.Cancel)
			{
				HostsManager.Disconnect();
                Properties.Settings.Default.Save();
			}
			else
				e.Cancel = true;
		}
		#endregion

		#region Rundown ownerdraw
		void lbRundown__DrawItem(object sender, DrawItemEventArgs e)
		{
            ListBox listbox = (ListBox)sender;
			if (e.Index >= 0)
			{
				RundownItem item = (RundownItem)listbox.Items[e.Index];

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
                    e.Graphics.FillRectangle(new SolidBrush(item.ChannelColor), e.Bounds.Right - 17, e.Bounds.Top, 17, e.Bounds.Height);
                    //Show warning when a channel is selected that is not availible   
                    if(!item.Online)
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

                        //cache channel-status
                        Hosts.ChannelInformation channelInfo = HostsManager.GetChannelInfo(item.Channel);
                        item.Online = (channelInfo != null) ? channelInfo.Online : false;
                        item.ChannelColor = (channelInfo != null) ? Color.FromArgb(channelInfo.ArgbValue) : Color.Red;

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

            if (returnvalue)
            {
                CurrentFilename = filename;
                AddToRecentFiles(filename);
            }

			return returnvalue;
		}

        private void AddToRecentFiles(string filename)
        {
            try
            {
                if (Properties.Settings.Default.RecentFiles.Contains(filename))
                    Properties.Settings.Default.RecentFiles.Remove(filename);
                Properties.Settings.Default.RecentFiles.Insert(0, filename);
                if (Properties.Settings.Default.RecentFiles.Count > 8)
                    Properties.Settings.Default.RecentFiles.RemoveAt(8);
            }
            catch { }
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
            
            if (returnvalue)
                AddToRecentFiles(filename);

			return returnvalue;
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
			        
		void DoClearLayer(RundownItem item)
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
        void DoClearChannel(RundownItem item)
        {
            if (item != null)
            {
                Svt.Caspar.Channel channel = HostsManager.GetChannel(item.Channel);
                if (channel != null)
                    channel.Clear();
            }
        }
		#endregion

        #region Shortcut keys handling
        const int WM_KEYDOWN = 0x100;
        const int WM_SYSKEYDOWN = 0x104;
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (msg.Msg == WM_SYSKEYDOWN || msg.Msg == WM_KEYDOWN)
            {
                switch (keyData)
                {
                    case Keys.Space:
                        if (IsOnAir)
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
                            return true;
                        }
                        break;

                    //case Keys.PageDown: //TODO: Convert to menuItem and change key!
                    //    DoPlayCurrentItem();
                    //    if (lbRundown_.Items.Count > lbRundown_.SelectedIndex + 1)
                    //    {
                    //        lbRundown_.SelectedIndex++;
                    //        DoPlayCurrentItem();
                    //        lbRundown_.SelectedIndex--;
                    //    }
                    //    return true;

                    //case Keys.End: //TODO: Convert to menuItem and change key!
                    //    {
                    //        int index1 = lbRundown_.SelectedIndex;
                    //        int index2 = lbRundown_.SelectedIndex + 1;
                    //        RundownItem item1 = lbRundown_.Items[index1] as RundownItem;
                    //        RundownItem item2 = lbRundown_.Items[index2] as RundownItem;

                    //        if (item1.IsCG)
                    //            DoStopItem(item1);
                    //        else
                    //            DoClear(item1);

                    //        if (item2.IsCG)
                    //            DoStopItem(item2);
                    //        else
                    //            DoClear(item2);
                    //    }
                    //    return true;

                    case (Keys.Control | Keys.X):
                        if (lbRundown_.Focused)
                        {
                            miCut_.PerformClick();
                            return true;
                        }
                        break;

                    case (Keys.Control | Keys.C):
                        if (lbRundown_.Focused)
                        {
                            miCopy_.PerformClick();
                            return true;
                        }
                        break;

                    case (Keys.Control | Keys.V):
                        if (lbRundown_.Focused)
                        {
                            miPaste_.PerformClick();
                            return true;
                        }
                        break;
                }
            }
            
            return base.ProcessCmdKey(ref msg, keyData);
        }
		#endregion

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
                    //subscribe to validConnectionChanged-events
                    device.VaildConnectionChanged += device_VaildConnectionChanged;

                    ToolStripMenuItem item = new ToolStripMenuItem();
                    item.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
                    item.Image = device.HasValidConnection ? Properties.Resources.connected : Properties.Resources.disconnected;
                    item.Name = device.Hostname;
                    item.Size = new System.Drawing.Size(100, 20);
                    item.Text = device.Hostname;
                    item.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
                    item.Click += new System.EventHandler(this.miDeviceStatus__Click);
                    miCasparStatus_.DropDownItems.Add(item);
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

                    //Update status of Caspar Host Status items
                    ToolStripItem[] items = miCasparStatus_.DropDownItems.Find(device.Hostname, false);
                    foreach (ToolStripItem item in items)
                        item.Image = device.HasValidConnection ? Properties.Resources.connected : Properties.Resources.disconnected;

                    //invalidate affected items in the rundown
                    int startIndex = lbRundown_.IndexFromPoint(0, 0);
                    int lastIndex = Math.Min(startIndex + lbRundown_.Height / lbRundown_.ItemHeight, lbRundown_.Items.Count - 1);
                    foreach (Hosts.ChannelInformation channel in device.Channels)
                    {
                        int itemCount = lbRundown_.Items.Count;
                        for (int index = 0; index < itemCount; ++index)
                        {
                            RundownItem item = (RundownItem)lbRundown_.Items[index];
                            if (item != null && item.Channel == channel.Label)
                            {
                                //cache channel-status in RundownItem
                                item.Online = device.HasValidConnection;
                                item.ChannelColor = Color.FromArgb(channel.ArgbValue);

                                //invalidate ItemRectangle if in view
                                if(index >= startIndex && index <= lastIndex)
                                    lbRundown_.Invalidate(lbRundown_.GetItemRectangle(index), false);
                            }
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

        void HostsManager_UpdatedTemplates(object sender, Hosts.HostEventArgs e)
        {
            itemViewControl_.UpdateTemplates();
        }

        void HostsManager_UpdatedMediafiles(object sender, Hosts.HostEventArgs e)
        {
            itemViewControl_.UpdateMediafiles();
        }

        #endregion
        
        #region Pages
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
        #endregion

        private void btnDetail__Click(object sender, EventArgs e)
        {
            if(btnDetail_.Checked == true)
                lbRundown_.ItemHeight = 64;
            else
                lbRundown_.ItemHeight = 32;
        }

        //////////////////////////
        //
        //  THE MENU-BAR
        //
        //////////////////////////
        #region file menu
        private void miNew__Click(object sender, EventArgs e)
        {
            if (CheckSaveBefore() != DialogResult.Cancel)
            {
                lbRundown_.Items.Clear();
                IsDirty = false;
            }
        }

        private void miLoadFile__Click(object sender, EventArgs e)
        {
            if (CheckSaveBefore() != DialogResult.Cancel)
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Multiselect = false;
                ofd.Filter = "Caspar rundown (*.crx)|*.crx";
                if (ofd.ShowDialog() == DialogResult.OK)
                    LoadFile(ofd.FileName);
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

            //hotkeys are now implemented as shortcutkeys to toolstripmenuitems
            sd.PlayoutMenuItems = playoutToolStripMenuItem;
            sd.CasparMenuItems = casparToolStripMenuItem;

            if (sd.ShowDialog() == DialogResult.OK)
            {
                itemViewControl_.PreviewControl.BackgroundColor = Properties.Settings.Default.PreviewBackgroundColor;
                SetPreviewControlPaths();

                itemViewControl_.UpdateDefaultColors();

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
            miLoadFile_.Enabled = miNewFile_.Enabled = miSettings_.Enabled = !IsOnAir;
        }

        private void recentFile__Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menuItem = (ToolStripMenuItem)sender;
            if (CheckSaveBefore() != DialogResult.Cancel)
            {
                string filename = menuItem.Tag.ToString();
                LoadFile(filename);
            }
        }
        private void miClearRecentFiles__Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.RecentFiles.Clear();
        }

        private void miRecentFiles__DropDownOpening(object sender, EventArgs e)
        {
            miClearRecentFiles_.Enabled = (Properties.Settings.Default.RecentFiles.Count != 0);

            for (int i = 0; i < 8; ++i)
            {
                if(i < Properties.Settings.Default.RecentFiles.Count)
                {
                    string filename = Properties.Settings.Default.RecentFiles[i];
                    miRecentFiles_.DropDownItems[i].Tag = filename;
                    miRecentFiles_.DropDownItems[i].Text = filename;//TruncatePath(filename, 60);
                    miRecentFiles_.DropDownItems[i].Visible = miRecentFiles_.DropDownItems[i].Enabled = true;
                }
                else
                    miRecentFiles_.DropDownItems[i].Visible = miRecentFiles_.DropDownItems[i].Enabled = false;

            }
        }

        //To truncate a filename
        /*[System.Runtime.InteropServices.DllImport( "shlwapi.dll" )]
        static extern bool PathCompactPathEx([System.Runtime.InteropServices.Out] StringBuilder pszOut, string szPath, int cchMax, int dwFlags);

        static string TruncatePath( string path, int length )
        {
            StringBuilder sb = new StringBuilder();
            PathCompactPathEx( sb, path, length, 0 );
            return sb.ToString();
        }*/
        #endregion

        #region edit menu
        RundownItem clipboard_ = null;
        private void miCut__Click(object sender, EventArgs e)
        {
            //TODO: Use the real clipboard instead. see below. doesn't work yet
            if (lbRundown_.SelectedItem != null)
            {
                clipboard_ = (RundownItem)(lbRundown_.SelectedItem as RundownItem).Clone();
                DeleteSelectedItem();
            }

            //if (lbRundown_.SelectedItem != null)
            //{
            //    RundownItem copy = (RundownItem)((RundownItem)lbRundown_.SelectedItem).Clone();
            //    Clipboard.SetData(RundownItemDataFormat.Name, copy);
            //    DeleteSelectedItem();
            //}
        }

        private void miCopy__Click(object sender, EventArgs e)
        {
            //TODO: Use the real clipboard instead. see below, doesn't work yet
            if (lbRundown_.SelectedItem != null)
            {
                clipboard_ = (RundownItem)(lbRundown_.SelectedItem as RundownItem).Clone();
            }
            
            //if (lbRundown_.SelectedItem != null)
            //{
            //    RundownItem copy = (RundownItem)((RundownItem)lbRundown_.SelectedItem).Clone();
            //    Clipboard.SetData(RundownItemDataFormat.Name, copy);
            //}
        }

        private void miPaste__Click(object sender, EventArgs e)
        {
            //TODO: Use the real clipboard instead. see below, doesn't work yet
            if (clipboard_ != null)
            {
                RundownItem newItem = clipboard_.Clone() as RundownItem;
                InsertItem(newItem);
            }

            //if (Clipboard.ContainsData(RundownItemDataFormat.Name))
            //{
            //    RundownItem copy = (RundownItem)Clipboard.GetData(RundownItemDataFormat.Name);
            //    if (copy != null)
            //    {
            //        RundownItem newItem = (RundownItem)copy.Clone();
            //        InsertItem(newItem);
            //    }
            //}
        }
        #endregion

        #region view menu
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

        private void templateDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.itemViewControl_.EnableTemplateData = !this.itemViewControl_.EnableTemplateData;
        }

        private void outputSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.itemViewControl_.EnableOutputSettings = !this.itemViewControl_.EnableOutputSettings;
        }
        #endregion

        #region playout menu
        private void miLoad__Click(object sender, EventArgs e)
        {
            DoLoadItem((RundownItem)lbRundown_.SelectedItem, false);
        }

        private void miPlay__Click(object sender, EventArgs e)
        {
            if (IsOnAir)
            {
                DoPlayItem((RundownItem)lbRundown_.SelectedItem);
            }
            else
            {
                DoPlayCurrentItem();
            }
        }

        private void miStep__Click(object sender, EventArgs e)
        {
            DoStepItem((RundownItem)lbRundown_.SelectedItem);
        }

        private void miUpdate__Click(object sender, EventArgs e)
        {
            DoUpdateItem((RundownItem)lbRundown_.SelectedItem);
        }

        private void miStop__Click(object sender, EventArgs e)
        {
            RundownItem item = (RundownItem)lbRundown_.SelectedItem;
            if (item != null)
            {
                if (item.IsCG)
                    DoStopItem(item);
                else
                    DoClearLayer(item);
            }
        }

        private void miClearLayer__Click(object sender, EventArgs e)
        {
            DoClearLayer((RundownItem)lbRundown_.SelectedItem);
        }

        private void miClearChannel__Click(object sender, EventArgs e)
        {
            DoClearChannel((RundownItem)lbRundown_.SelectedItem);
        }

        private void miClearAll__Click(object sender, EventArgs e)
        {
            foreach (Hosts.ChannelInformation channelInfo in HostsManager.ChannelInfos)
            {
                if (channelInfo.Online)
                {
                    Svt.Caspar.Channel channel = HostsManager.GetChannel(channelInfo.Label);
                    if (channel != null)
                        channel.Clear();
                }
            }

        }
        #endregion

        #region caspar menu
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

        #region help menu
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutDialog dialog = new AboutDialog();
            dialog.ShowDialog();
        }
        #endregion

    }
}
