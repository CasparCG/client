using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System;
using System.ComponentModel;

namespace Caspar_Pilot
{
	/// <summary>
	/// Summary description for DnDListBox.
	/// </summary>
	public class DnDListBox<T> : System.Windows.Forms.ListBox where T : ICloneable
	{
		private string dragDropDataFormat_ = DataFormats.Text;
		private DataFormats.Format dataFormat_ = null;


		private System.Drawing.Rectangle dragBox = System.Drawing.Rectangle.Empty;
		private int dragSourceItemIndex = ListBox.NoMatches;
		private int dragOverIndex_ = ListBox.NoMatches;
		private bool bIsDragSource = false;
		private bool bReorderDrop = false;

		protected new event MouseEventHandler MouseDown;
		protected new event MouseEventHandler MouseMove;
		protected new event MouseEventHandler MouseUp;
		public event EventHandler<DataEventArgs<T>> DragMoved;

		public DnDListBox()
		{
            this.DoubleBuffered = true;
			dataFormat_ = DataFormats.GetFormat(DragDropDataFormat);
		}

		public string DragDropDataFormat
		{
			get { return dragDropDataFormat_; }
			set {
					dataFormat_ = DataFormats.GetFormat(value);
					dragDropDataFormat_ = value;
				}
		}
		public int DragOverIndex
		{
			get { return dragOverIndex_; }
		}
		public bool IsDragSource {
			get { return bIsDragSource; }
		}

		#region Drag And Drop Events
		protected override void OnDragDrop(DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DragDropDataFormat))
			{
				T obj = (T)e.Data.GetData(DragDropDataFormat);

				if(bIsDragSource && this.DragOverIndex == dragSourceItemIndex) {
					e.Effect = DragDropEffects.None;
					this.SelectedIndex = dragSourceItemIndex;
				}

				else if(this.DragOverIndex == ListBox.NoMatches) {
					if(e.Effect == DragDropEffects.Copy)
						obj = (T)obj.Clone();

					//Put the dropped item at the end of the list
					this.Items.Add(obj);

					//select the newly inserted item
					this.SelectedIndex = this.Items.Count-1;
				}
				else {
					if(bIsDragSource && this.DragOverIndex < dragSourceItemIndex)
						++dragSourceItemIndex;

					if(e.Effect == DragDropEffects.Copy)
						obj = (T)obj.Clone();

					//Put the dropped item before the hovered index
					this.Items.Insert(this.DragOverIndex, obj);

					//select the newly inserted item
					this.SelectedIndex = this.DragOverIndex;
				}

				if(bIsDragSource && e.Effect == DragDropEffects.Move)
					bReorderDrop = true;

				//we need to make sure that the reference in the dataobject is the same as the one that was inserted in the list (even if we inserted a copy)
				e.Data.SetData(DragDropDataFormat, false, obj);
			}

			base.OnDragDrop(e);
		}

		protected override void OnDragOver(DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DragDropDataFormat))
			{
				if(((e.KeyState & 8) == 8) || ((e.KeyState & 4) == 4))	//SHIFT or CTRL
					e.Effect = DragDropEffects.Copy;
				else
					e.Effect = DragDropEffects.Move;

				Point position = this.PointToClient(new Point(e.X, e.Y));
				dragOverIndex_ = this.IndexFromPoint(position);
			}

			base.OnDragOver(e);
		}

		protected override void OnQueryContinueDrag(QueryContinueDragEventArgs e)
		{
			if(e.EscapePressed) 
				e.Action = DragAction.Cancel;

			base.OnQueryContinueDrag(e);
		}


		#endregion Drag And Drop Events

		#region Mouse Events
		protected override void OnMouseDown(MouseEventArgs e)
		{
			this.SelectedIndex = this.IndexFromPoint(e.X, e.Y);
			this.OnSelectedIndexChanged(new EventArgs());

			if((e.Button & MouseButtons.Left) == MouseButtons.Left)
			{
				dragSourceItemIndex = this.IndexFromPoint(e.X, e.Y);
				if(dragSourceItemIndex != ListBox.NoMatches)
				{
					Size dragSize = SystemInformation.DragSize;
					dragBox = new Rectangle(new Point(e.X - (dragSize.Width /2), e.Y - (dragSize.Height /2)), dragSize);
				}
				else
					dragBox = Rectangle.Empty;
			}
			
			if(MouseDown != null)
				MouseDown(this, e);
		}

		protected override void OnMouseMove(MouseEventArgs e)
		{
			if((e.Button & MouseButtons.Left) == MouseButtons.Left)
			{
				if(dragSourceItemIndex != ListBox.NoMatches && !dragBox.IsEmpty && !dragBox.Contains(e.X, e.Y))
				{
					bIsDragSource = true;
					bReorderDrop = false;
					DataObject dataObject = new DataObject();
					T obj = (T)this.Items[dragSourceItemIndex];
					dataObject.SetData(DragDropDataFormat, false, obj);
					DragDropEffects dropEffect = DoDragDrop(dataObject, DragDropEffects.Move | DragDropEffects.Copy);
					if(dropEffect == DragDropEffects.Move)
					{
						this.Items.RemoveAt(dragSourceItemIndex);
						
						if(!bReorderDrop)
							OnDragMoved(obj);
					}
					bReorderDrop = false;
					bIsDragSource = false;
					dragSourceItemIndex = ListBox.NoMatches;
					dragBox = Rectangle.Empty;
				}
			}
			else if((e.Button & MouseButtons.Right) == MouseButtons.Right)
			{
				this.SelectedIndex = this.IndexFromPoint(e.X, e.Y);
			}	
			
			if(MouseMove != null)
				MouseMove(this, e);
		}
		
		private void OnDragMoved(T obj) {
			if(DragMoved != null) {
				DragMoved(this, new DataEventArgs<T>(obj));
			}
		}

		protected override void OnMouseUp(MouseEventArgs e)
		{
			if((e.Button & MouseButtons.Left) == MouseButtons.Left)
			{
				dragSourceItemIndex = ListBox.NoMatches;
				dragBox = Rectangle.Empty;
			}
			else if((e.Button & MouseButtons.Right) == MouseButtons.Right)
			{
				if(this.SelectedIndex != ListBox.NoMatches)
				{
					if(this.ContextMenu != null)
						this.ContextMenu.Show(this, new Point(e.X, e.Y));
				}
			}
	
			if(MouseUp != null)
				MouseUp(this, e);
		}
		#endregion Mouse Events

		private void InitializeComponent()
		{
            this.SuspendLayout();
            this.ResumeLayout(false);

		}
	}
}
