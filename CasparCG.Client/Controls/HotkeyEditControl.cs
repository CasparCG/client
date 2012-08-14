using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Caspar_Pilot.Controls
{
    public partial class HotkeyEditControl : UserControl
    {
        TypeConverter converter = null;
        
        public HotkeyEditControl()
        {
            InitializeComponent();
            InitializeDropdown();

            converter = TypeDescriptor.GetConverter(typeof(Keys));
        }

        private void InitializeDropdown()
        {
            cKey.Items.Add(Keys.None);
            cKey.Items.Add(Keys.Pause);
            cKey.Items.Add(Keys.Escape);
            cKey.Items.Add(Keys.PageUp);
            cKey.Items.Add(Keys.PageDown);
            cKey.Items.Add(Keys.Home);
            cKey.Items.Add(Keys.End);
            cKey.Items.Add(Keys.Left);
            cKey.Items.Add(Keys.Up);
            cKey.Items.Add(Keys.Right);
            cKey.Items.Add(Keys.Down);
            cKey.Items.Add(Keys.Insert);
            cKey.Items.Add(Keys.Delete);

            for(int i = (int)Keys.D0; i <= (int)Keys.D9; ++i)
                cKey.Items.Add((Keys)i);

            for (int i = (int)Keys.A; i <= (int)Keys.Z; ++i)
                cKey.Items.Add((Keys)i);

            for (int i = (int)Keys.F1; i <= (int)Keys.F12; ++i)
                cKey.Items.Add((Keys)i);
        }


        public override string Text
        {
            get
            {
                Keys key = (Keys)cKey.SelectedItem;
                if (key != Keys.None)
                {
                    if (cbAlt.Checked) key |= Keys.Alt;
                    if (cbCtrl.Checked) key |= Keys.Control;
                    if (cbShift.Checked) key |= Keys.Shift;
                }

                return converter.ConvertToString(key);
            }
            set
            {
                Keys key = (Keys)converter.ConvertFromString(value);
                cbAlt.Checked = (key & Keys.Alt) == Keys.Alt;
                cbCtrl.Checked = (key & Keys.Control) == Keys.Control;
                cbShift.Checked = (key & Keys.Shift) == Keys.Shift;

                cKey.SelectedItem = (key & Keys.KeyCode);
            }
        }

        private void cKey_SelectedIndexChanged(object sender, EventArgs e)
        {
            Keys key = (Keys)cKey.SelectedItem;
            if (key == Keys.None)
            {
                cbAlt.Checked = cbCtrl.Checked = cbShift.Checked = false;
            }
        }
    }
}
