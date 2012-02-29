using System;
using System.Collections.Generic;
using System.Text;

namespace Caspar_Pilot
{
	public class DataEventArgs<T> : EventArgs
	{
		public DataEventArgs(T data)
		{
			Data = data;
		}

		public T Data { get; set; }
	}
}
