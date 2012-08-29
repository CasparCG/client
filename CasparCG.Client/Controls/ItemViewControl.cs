using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using Settings = Caspar_Pilot.Properties.Settings;
using Svt.Caspar.Controls;
using Svt.Caspar;

namespace Caspar_Pilot.Controls
{
	public partial class ItemViewControl : UserControl
	{
		static List<Svt.Caspar.CGDataPair> dummyList_ = new List<Svt.Caspar.CGDataPair>();

		public delegate void SimpleDelegate();
		private List<string> templatePaths = new List<string>();

		public Svt.Caspar.Controls.FlashTemplateHostControl PreviewControl
		{
			get;
			private set;
		}

		public bool EnablePreview
		{
            get { return gbPreview_.Visible; }
			set
            {
                gbPreview_.Visible = value;
                this.tableLayoutPanel1.RowStyles[0].Height = (value == true) ? 50F : 0;
                tTemplateHostVersion.Text = FlashTemplateHostControl.GetVersionString(PreviewControl.Version);

                Properties.Settings.Default.EnablePreview = value;
            }
		}

        public bool EnableVideoLayers
        {
            get { return numericUpDownVideoLayer.Enabled; }
            set { numericUpDownVideoLayer.Enabled = value; }
        }

        public bool EnableControlValues
        {
            set
            {
                if (!value)
                    this.tcItemSettings_.TabPages.Remove(this.tabPageControl);
            }
        }

        public bool EnableTemplateData
        {
            get { return groupBoxTemplteData.Visible; }
            set
            {
                groupBoxTemplteData.Visible = value;

                Properties.Settings.Default.EnableTemplateData = value;
            }
        }

        public bool EnableOutputSettings
        {
            get { return groupBoxOutputSettings.Visible; }
            set
            {
                groupBoxOutputSettings.Visible = value;
                this.tableLayoutPanel1.ColumnStyles[0].Width = (value == true) ? 270 : 0;

                Properties.Settings.Default.EnableOutputSettings = value;
            }
        }

		public event EventHandler<EventArgs> CurrentItemDataChanged;
		protected void OnCurrentItemDataChanged()
		{
            try
            {
                if (CurrentItemDataChanged != null)
                    CurrentItemDataChanged(this, EventArgs.Empty);
            }
            catch { }
		}

        public event EventHandler<EventArgs> CurrentItemChannelChanged;
        protected void OnCurrentItemChannelChanged()
        {
            try
            {
                if (CurrentItemChannelChanged != null)
                    CurrentItemChannelChanged(this, EventArgs.Empty);
            }
            catch { }
        }

        public Color TemplateColor { get; set; }
        public Color GraphicColor { get; set; }
        public Color ControlColor { get; set; }
        public Color MultistepColor { get; set; }
		public Color StoredDataColor { get; set; }
        public Color DataUpdateColor { get; set; }
        internal OperatorForm OperatorForm { get; set; }

		public ItemViewControl()
		{
			InitializeComponent();
			InitializeCustomComponents();

            UpdateDefaultColors();

			dataviewBindingSource.AddingNew += new AddingNewEventHandler(dataviewBindingSource_AddingNew);
			cbTransition_.Items.AddRange(Enum.GetNames(typeof(Svt.Caspar.TransitionType)));
			cbTransition_.SelectedIndex = 0;

            LoadControlSettings();
		}

        private void LoadControlSettings()
        {
            foreach(string setting in Settings.Default.SpyderSettings)
            {
                this.comboBoxSettings.Items.Add(setting.Split('#')[0]);
            }
        }

		private void InitializeCustomComponents()
		{
			PreviewControl = new Svt.Caspar.Controls.FlashTemplateHostControl();
			PreviewControl.Dock = DockStyle.Fill;
			pPreviewContainer_.Controls.Add(PreviewControl);
		}

		RundownItem currentItem_ = null;
		internal RundownItem CurrentItem
		{
			get { return currentItem_; }
			set
			{
				RundownItem lastItem = currentItem_;

				bool bDoUpdateItemChannel = false;
				currentItem_ = null;

				if (value != null)
				{
					tcItemSettings_.SelectedIndex = (value.IsCG ? 0 : 1);
                    PreviewControl.Visible = value.IsCG;
                    if (value.IsControl)
                    {
                        tcItemSettings_.SelectedIndex = 2;
                        comboBoxSettings.SelectedItem = value.StaticItem.Clipname;
                        comboBoxSystemControl.SelectedItem = value.Channel;
                        numericUpDownVideoLayer.Value = value.StaticItem.VideoLayer;
                    }
					else if (value.IsCG)
					{
                        cbMediafiles_.Text = value.StaticItem.Clipname;
                        numericUpDownVideoLayer.Value = value.CGItem.VideoLayer;
						if(value != lastItem)
							PreviewControl.PrepareNewFlash();

						if (!dataGridView1.Visible)
							dataGridView1.Visible = true;
					}
					else
					{
                        cbMediafiles_.Text = value.StaticItem.Clipname;
                        numericUpDownVideoLayer.Value = value.StaticItem.VideoLayer;
						if (dataGridView1.Visible)
							dataGridView1.Visible = false;
					}

					tName_.Text = value.Name;

					bDoUpdateItemChannel = SetSelectedChannel(value);

					cbTemplates_.Text = value.CGItem.TemplateIdentifier;
					nLayer_.Value = value.CGItem.Layer;
					cMultistep_.Checked = value.MultiStep;
					checkBoxStoredData.Checked = value.IsStoredData;
                    cbUpdateData.Checked = value.IsDataUpdate;
                    cLoop_.Checked = value.StaticItem.Loop;
					cbTransition_.SelectedIndex = (int)value.StaticItem.Transition.Type;
					nDuration_.Value = value.StaticItem.Transition.Duration;
					dataviewBindingSource.DataSource = value.CGItem.Data;
					dataviewBindingSource.AllowNew = true;
                    cbColor_.Text = value.Color.Name;
                    textBoxPage.Text = value.Page;
                    numericUpDownControlValue.Value = value.ControlValue;
				}
				else
					dataviewBindingSource.DataSource = dummyList_;

				if (value == null)
					dataGridView1.Visible = false;

				tableLayoutPanel1.Enabled = (value != null);
				currentItem_ = value;
				if (bDoUpdateItemChannel)
				{
					cbChannels__SelectedIndexChanged(this, EventArgs.Empty);
				}
				UpdatePreviewVisibility();
			}
		}

		private bool SetSelectedChannel(RundownItem item)
		{
            if (item.IsControl)
                return false;

			if (string.IsNullOrEmpty(item.Channel))
			{
				if (cbChannels_.Items.Count > 0)
				{
					cbChannels_.SelectedIndex = 0;
					return true;
				}
			}
			else
			{
				if (cbChannels_.Items.Contains(item.Channel))
					cbChannels_.SelectedIndex = cbChannels_.Items.IndexOf(item.Channel);
				else
				{
					cbChannels_.Items.Add(item.Channel);
					cbChannels_.SelectedIndex = cbChannels_.Items.Count - 1;
				}
			}
			return false;
		}

		#region Preview
		static int PreviewLayer = 1;
		private void UpdatePreviewVisibility()
		{
			if (CurrentItem != null && CurrentItem.IsCG && CurrentItem.CGItem != null && PreviewControl.Valid)
			{
				string fullFilename = System.IO.Path.Combine(PreviewControl.TemplateFolder, CurrentItem.CGItem.TemplateIdentifier + ".ft");
				if(System.IO.File.Exists(fullFilename))
				{
					btnPreviewPlay_.Enabled = true;
					return;
				}
			}
			btnPreviewPlay_.Enabled = false;
		}

		private void btnPreviewPlay__Click(object sender, EventArgs e)
		{
			if (CurrentItem != null && CurrentItem.CGItem != null)
			{
				if (PreviewControl.Add(CurrentItem.CGItem, PreviewLayer))
				{
					lPreviewInfo_.Text = "Preview " + CurrentItem.CGItem.TemplateIdentifier;
					btnPreviewStep_.Enabled = true;
				}
			}
		}

		private void btnPreviewStop__Click(object sender, EventArgs e)
		{
			PreviewControl.Stop(PreviewLayer);
			lPreviewInfo_.Text = "";
			btnPreviewStep_.Enabled = false;
		}

		private void btnPreviewStep__Click(object sender, EventArgs e)
		{
			if (CurrentItem != null && CurrentItem.CGItem != null)
			{
				PreviewControl.Next(PreviewLayer);
			}
		}
		#endregion

		public void UpdateMediafiles()
		{
			if (InvokeRequired)
				BeginInvoke(new SimpleDelegate(UpdateMediafiles));
			else
			{
				cbMediafiles_.Items.Clear();
				string selectedChannel = (string)cbChannels_.SelectedItem;
				if (!string.IsNullOrEmpty(selectedChannel))
				{
					Hosts.ChannelInformation channelInfo = OperatorForm.HostsManager.GetChannelInfo(selectedChannel);
					if (channelInfo != null)
					{
						Hosts.DeviceHolder device = OperatorForm.HostsManager.GetDevice(channelInfo.Hostname);
						if (device != null)
						{
                            cbMediaCategory_.SuspendLayout();
                            cbMediaCategory_.Items.Clear();
                            cbMediaCategory_.Items.Add("_ALL_");
                            cbMediaCategory_.SelectedItem = cbMediaCategory_.Items[0];
  
                            try
                            {
                                foreach (MediaInfo media in device.Files)
                                {
                                    if (!string.IsNullOrEmpty(media.Folder) && !cbMediaCategory_.Items.Contains(media.Folder))
                                        cbMediaCategory_.Items.Add(media.Folder);
                                }
                            }
                            finally
                            {
                                cbMediaCategory_.ResumeLayout();
                            }

                            UpdateMediaListBox();
							//cbMediafiles_.Items.AddRange(device.Files.ToArray());
						}
					}
				}
			}
		}

		Svt.Caspar.TemplatesCollection templatesCollection_ = null;
		public void UpdateTemplates()
		{
			if (InvokeRequired)
				BeginInvoke(new SimpleDelegate(UpdateTemplates));
			else
			{
				string selectedChannel = (string)cbChannels_.SelectedItem;
				if (!string.IsNullOrEmpty(selectedChannel))
				{
					Hosts.ChannelInformation channelInfo = OperatorForm.HostsManager.GetChannelInfo(selectedChannel);
					if (channelInfo != null)
					{
						Hosts.DeviceHolder device = OperatorForm.HostsManager.GetDevice(channelInfo.Hostname);
						if (device != null)
						{
							cbCategory_.SuspendLayout();
							cbCategory_.Items.Clear();
							cbCategory_.Items.Add("_ALL_");
							cbCategory_.SelectedItem = cbCategory_.Items[0];
							templatePaths.Clear();

							try
							{
								templatesCollection_ = device.Templates;
								if (templatesCollection_ != null)
								{
									foreach (string folder in templatesCollection_.Folders)
									{
										List<Svt.Caspar.TemplateInfo> templates = templatesCollection_.GetTemplatesInFolder(folder);
										foreach (Svt.Caspar.TemplateInfo template in templates)
										{
											string fullpath = "";
											if (!string.IsNullOrEmpty(template.Folder))
												fullpath = template.Folder.Replace('\\', '/') + '/' + template.Name;
											else
												fullpath = template.Name;

											templatePaths.Add(fullpath);

											if (fullpath.IndexOf('/') != -1)
											{
												string category = fullpath.Substring(0, fullpath.IndexOf('/'));
												if (!cbCategory_.Items.Contains(category))
													cbCategory_.Items.Add(category);
											}
										}
									}
								}
							}
							finally
							{
								cbCategory_.ResumeLayout();
							}
							UpdateTemplateListBox();
						}
					}
				}
			}
		}

        private void UpdateTemplateListBox()
        {
            cbTemplates_.SuspendLayout();
            cbTemplates_.Items.Clear();

            try
            {
                string currentCategory = (string)cbCategory_.SelectedItem;
                foreach (string fullpath in templatePaths)
                {
                    if (fullpath.IndexOf('/') != -1)
                    {
                        string category = fullpath.Substring(0, fullpath.IndexOf('/'));
                        if (currentCategory == category)
                            cbTemplates_.Items.Add(fullpath);
                    }

                    if (currentCategory == "_ALL_")
                        cbTemplates_.Items.Add(fullpath);
                }
            }
            finally
            {
                cbTemplates_.ResumeLayout();
            }
        }

        private void UpdateMediaListBox()
        {
            cbMediafiles_.SuspendLayout();
            cbMediafiles_.Items.Clear();

            try
            {
                string selectedChannel = (string)cbChannels_.SelectedItem;
                if (!string.IsNullOrEmpty(selectedChannel))
                {
                    Hosts.ChannelInformation channelInfo = OperatorForm.HostsManager.GetChannelInfo(selectedChannel);
                    if (channelInfo != null)
                    {
                        Hosts.DeviceHolder device = OperatorForm.HostsManager.GetDevice(channelInfo.Hostname);
                        string currentCategory = (string)cbMediaCategory_.SelectedItem;
                        foreach (MediaInfo media in device.Files)
                        {
                            if (!string.IsNullOrEmpty(media.Folder) && media.Folder == currentCategory)
                                cbMediafiles_.Items.Add(media.FullName.Replace('\\', '/'));

                            if (currentCategory == "_ALL_")
                                cbMediafiles_.Items.Add(media.FullName.Replace('\\', '/'));
                        }
                    }
                }
            }
            finally
            {
                cbMediafiles_.ResumeLayout();
            }
        }

		internal void SetChannels(IEnumerable<Hosts.ChannelInformation> channels)
		{
			cbChannels_.SuspendLayout();
			try
			{
				cbChannels_.Items.Clear();
				//cbChannels_.Items.Add(string.Empty);
				if (channels != null)
				{
					foreach (Hosts.ChannelInformation info in channels)
						cbChannels_.Items.Add(info.Label);
				}
			}
			finally
			{
				cbChannels_.ResumeLayout();
			}

			if (CurrentItem != null)
				SetSelectedChannel(CurrentItem);
		}

		#region control notifications
		private void tcItemSettings__SelectedIndexChanged(object sender, EventArgs e)
		{
            if (CurrentItem != null && tcItemSettings_.Focused)
			{
                bool wasCG = CurrentItem.IsCG;
				CurrentItem.IsCG = (tcItemSettings_.SelectedIndex == 0);
                CurrentItem.IsControl = (tcItemSettings_.SelectedIndex == 2);
				dataGridView1.Visible = CurrentItem.IsCG;
                CurrentItem.Channel = cbChannels_.Text;
                
                
                if (CurrentItem.IsControl)
                {
                    CurrentItem.StaticItem.Clipname = this.comboBoxSettings.Text;
                    CurrentItem.StaticItem.VideoLayer = Convert.ToInt32(this.numericUpDownVideoLayer.Value);
                    CurrentItem.Channel = this.comboBoxSystemControl.Text;
                    CurrentItem.Color = ControlColor;
                    CurrentItem.ControlValue = int.Parse(numericUpDownControlValue.Value.ToString());
                }
                else if (CurrentItem.IsCG && !wasCG)
                {
                    CurrentItem.CGItem.VideoLayer = Convert.ToInt32(this.numericUpDownVideoLayer.Value);

                    if(CurrentItem.MultiStep)
                        CurrentItem.Color = MultistepColor; 
                    else
                        CurrentItem.Color = TemplateColor;

                    cbColor_.Text = CurrentItem.Color.Name;
                }
                else if (!CurrentItem.IsCG == wasCG)
                {
                    CurrentItem.StaticItem.Clipname = this.cbMediafiles_.Text;
                    CurrentItem.StaticItem.VideoLayer = Convert.ToInt32(this.numericUpDownVideoLayer.Value);
                    CurrentItem.Color = GraphicColor;
                    cbColor_.Text = CurrentItem.Color.Name;
                }
                else
                {
                    CurrentItem.StaticItem.Clipname = this.cbMediafiles_.Text;
                    CurrentItem.StaticItem.VideoLayer = Convert.ToInt32(this.numericUpDownVideoLayer.Value);
                    if (tcItemSettings_.SelectedIndex == 0)
                        CurrentItem.Color = TemplateColor;
                    else if (tcItemSettings_.SelectedIndex == 1)
                        CurrentItem.Color = GraphicColor;
                }

				UpdatePreviewVisibility();
				OnCurrentItemDataChanged();
			}
		}

		private void tName__TextChanged(object sender, EventArgs e)
		{
			TextBox tb = sender as TextBox;
			if (CurrentItem != null)
			{
				CurrentItem.Name = tb.Text;
				OnCurrentItemDataChanged();
			}
		}

		private void nLayer__ValueChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.CGItem.Layer = Convert.ToInt32(nLayer_.Value);
				OnCurrentItemDataChanged();
			}
		}

		private void cbTransition__SelectedIndexChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null && cbTransition_.SelectedIndex != ListBox.NoMatches)
			{
				CurrentItem.StaticItem.Transition.Type = (Svt.Caspar.TransitionType)Enum.Parse(typeof(Svt.Caspar.TransitionType), cbTransition_.SelectedItem.ToString());
				OnCurrentItemDataChanged();
			}
		}

		private void nDuration__ValueChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.StaticItem.Transition.Duration = Convert.ToInt32(nDuration_.Value);
				OnCurrentItemDataChanged();
			}
		}

		private void cbMediafiles__SelectedIndexChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null && cbMediafiles_.SelectedIndex != ListBox.NoMatches)
			{
				CurrentItem.StaticItem.Clipname = cbMediafiles_.SelectedItem.ToString();
				OnCurrentItemDataChanged();
			}
		}

		private void cbMediafiles__TextUpdate(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.StaticItem.Clipname = cbMediafiles_.Text;
				OnCurrentItemDataChanged();
			}
		}

		private void cbTemplates__SelectedIndexChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null && cbTemplates_.SelectedIndex != ListBox.NoMatches)
			{
				CurrentItem.CGItem.TemplateName = cbTemplates_.SelectedItem.ToString();
				UpdatePreviewVisibility();
				OnCurrentItemDataChanged();
			}
		}

		private void cbTemplates__TextUpdate(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.CGItem.TemplateName = cbTemplates_.Text;
				UpdatePreviewVisibility();
				OnCurrentItemDataChanged();
			}
		}

		private void cMultistep__CheckedChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.MultiStep = cMultistep_.Checked;
				if (CurrentItem.MultiStep)
					CurrentItem.Color = MultistepColor;
				else
					CurrentItem.Color = TemplateColor;

				cbColor_.Text = CurrentItem.Color.Name;
			}
		}

		private void dataGridView1_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
		{
			e.Cancel = (e.Row.Index != (dataGridView1.RowCount-2));
		}

		void dataviewBindingSource_AddingNew(object sender, AddingNewEventArgs e)
		{
			if (CurrentItem != null)
				e.NewObject = new Svt.Caspar.CGDataPair(Properties.Settings.Default.DefaultFieldNamePrefix + (CurrentItem.CGItem.Data.Count + Properties.Settings.Default.DefaultFieldNameStartIndex), "");
			else
				e.NewObject = new Svt.Caspar.CGDataPair("", "");
		}

		private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
		{
			if (dataGridView1.Rows.Count > 0 && currentItem_ != null)
			{
				string value = dataGridView1.Rows[0].Cells[1].Value as string;
				if (string.IsNullOrEmpty(currentItem_.Name) && value != "")
					tName_.Text = value;
			}
		}

		private void cbCategory__SelectedIndexChanged(object sender, EventArgs e)
		{
			UpdateTemplateListBox();
		}

		private void cbColor__TextChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				try
				{
					CurrentItem.Color = Color.FromName(cbColor_.Text);
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
		}

		private void checkBox1_CheckedChanged(object sender, EventArgs e)
		{
			if (CurrentItem != null)
			{
				CurrentItem.StaticItem.Loop = cLoop_.Checked;
				OnCurrentItemDataChanged();
			}
		}

		private void cbChannels__SelectedIndexChanged(object sender, EventArgs e)
		{
            if (CurrentItem != null && !CurrentItem.IsControl  && cbChannels_.SelectedIndex != ListBox.NoMatches)
			{
				string label = (string)cbChannels_.SelectedItem;
				CurrentItem.Channel = label;
                OnCurrentItemChannelChanged();
				OnCurrentItemDataChanged();
			}
			UpdateMediafiles();
			UpdateTemplates();
		}
		#endregion

		internal void LoadTemplatesFromOfflinefolder()
		{
            cbCategory_.SuspendLayout();
            cbCategory_.Items.Clear();
            cbCategory_.Items.Add("_ALL_");
            cbCategory_.SelectedItem = cbCategory_.Items[0];
            templatePaths.Clear();

			try
			{
				foreach (string file in System.IO.Directory.GetFiles(PreviewControl.TemplateFolder, "*.ft", System.IO.SearchOption.AllDirectories))
				{
					int startIndex = PreviewControl.TemplateFolder.Length + 1;
					int lastIndex = file.Length - 3;
					string fullpath = file.Substring(startIndex, lastIndex - startIndex).Replace('\\', '/').ToUpper();
					templatePaths.Add(fullpath);

					if (fullpath.IndexOf('/') != -1)
					{
						string category = fullpath.Substring(0, fullpath.IndexOf('/'));
						if (!cbCategory_.Items.Contains(category))
							cbCategory_.Items.Add(category);
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Failed to get template list from offline folder");
			}
			finally
			{
				cbCategory_.ResumeLayout();
			}
		
            UpdateTemplateListBox();			
		}

		private void checkBoxStoredData_CheckedChanged(object sender, EventArgs e)
		{
			if (this.CurrentItem != null)
			{
				this.CurrentItem.IsStoredData = (sender as CheckBox).Checked;
				if (this.CurrentItem.IsStoredData)
					this.CurrentItem.Color = this.StoredDataColor;
				else
					this.CurrentItem.Color = TemplateColor;

				this.cbColor_.Text = this.CurrentItem.Color.Name;
			}
		}

        private void cbUpdateData_CheckedChanged(object sender, EventArgs e)
        {
            if (this.CurrentItem != null)
            {
                this.CurrentItem.IsDataUpdate = (sender as CheckBox).Checked;
                OnCurrentItemDataChanged();
            }
        }

		private void dataGridView1_Leave(object sender, EventArgs e)
		{
			if (dataGridView1.CurrentCell != null && dataGridView1.CurrentCell.IsInEditMode)
			{
				dataGridView1.EndEdit();
			}
		}

        private void textBoxPage_TextChanged(object sender, EventArgs e)
        {
            if (CurrentItem != null)
            {
                CurrentItem.Page = textBoxPage.Text;
                OnCurrentItemDataChanged();
            }
        }

        private void comboBoxSettings_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CurrentItem != null && comboBoxSettings.SelectedIndex != ListBox.NoMatches)
            {
                CurrentItem.StaticItem.Clipname = comboBoxSettings.SelectedItem.ToString();
                OnCurrentItemDataChanged();
            }
        }

        private void numericUpDownVideoLayer_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentItem != null)
            {
                if (CurrentItem.IsCG)
                    CurrentItem.CGItem.VideoLayer = Convert.ToInt32(numericUpDownVideoLayer.Value);
                else
                    CurrentItem.StaticItem.VideoLayer = Convert.ToInt32(numericUpDownVideoLayer.Value);

                OnCurrentItemDataChanged();
            }
        }

        private void numericUpDownControlValue_ValueChanged(object sender, EventArgs e)
        {
            if (CurrentItem != null)
            {
                CurrentItem.ControlValue = Convert.ToInt32(numericUpDownControlValue.Value);

                OnCurrentItemDataChanged();
            }
        }

        private void comboBoxSystemControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CurrentItem != null)
            {
                CurrentItem.Channel = comboBoxSystemControl.Text;
                OnCurrentItemDataChanged();
            }
        }

        private void cbMediaCategory__SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateMediaListBox();
        }

        private void toolStripMenuItemAdd_Click(object sender, EventArgs e)
        {
            this.dataviewBindingSource.Add(new CGDataPair(Properties.Settings.Default.DefaultFieldNamePrefix + (CurrentItem.CGItem.Data.Count + Properties.Settings.Default.DefaultFieldNameStartIndex), ""));
        }

        private void toolStripMenuItemInsert_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.CurrentRow == null)
                return;

            this.dataviewBindingSource.Insert(this.dataGridView1.CurrentRow.Index, new CGDataPair("", ""));
        }

        private void toolStripMenuItemRemove_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.CurrentRow == null)
                return;

            this.dataviewBindingSource.RemoveAt(this.dataGridView1.CurrentRow.Index);
        }


        internal void UpdateDefaultColors()
        {
            TemplateColor = Color.FromName(Properties.Settings.Default.TemplateColor);
            GraphicColor = Color.FromName(Properties.Settings.Default.GraphicColor);
            ControlColor = Color.FromName(Properties.Settings.Default.ControlColor);
            MultistepColor = Color.FromName(Properties.Settings.Default.MultistepColor);
            StoredDataColor = Color.FromName(Properties.Settings.Default.StoredDataColor);
            DataUpdateColor = Color.FromName(Properties.Settings.Default.DataUpdateColor);
        }
    }
}
