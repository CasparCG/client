using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace Caspar_Pilot
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new OperatorForm());
            }
            catch (COMException ex)
            {
                MessageBox.Show("An error occurred when loading flash player. Do you have flash player installed?", "CasparCG Client", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
		}
	}
}
