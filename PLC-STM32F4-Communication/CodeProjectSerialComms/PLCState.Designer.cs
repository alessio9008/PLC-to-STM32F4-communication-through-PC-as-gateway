namespace CodeProjectSerialComms
{
    partial class PLCState
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
            this.richPLCOutput = new System.Windows.Forms.RichTextBox();
            this.pbMove = new System.Windows.Forms.PictureBox();
            this.TmPictureBoxImage = new System.Windows.Forms.Timer(this.components);
            this.btnPLCClear = new System.Windows.Forms.Button();
            this.chdoNotWaitComplete = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.pbMove)).BeginInit();
            this.SuspendLayout();
            // 
            // richPLCOutput
            // 
            this.richPLCOutput.Location = new System.Drawing.Point(718, 12);
            this.richPLCOutput.Name = "richPLCOutput";
            this.richPLCOutput.Size = new System.Drawing.Size(477, 450);
            this.richPLCOutput.TabIndex = 0;
            this.richPLCOutput.Text = "";
            // 
            // pbMove
            // 
            this.pbMove.BackColor = System.Drawing.Color.Transparent;
            this.pbMove.BackgroundImage = global::CodeProjectSerialComms.Properties.Resources.BaseBig;
            this.pbMove.Image = global::CodeProjectSerialComms.Properties.Resources.Braccio1big;
            this.pbMove.Location = new System.Drawing.Point(12, 12);
            this.pbMove.Name = "pbMove";
            this.pbMove.Size = new System.Drawing.Size(700, 545);
            this.pbMove.TabIndex = 1;
            this.pbMove.TabStop = false;
            // 
            // TmPictureBoxImage
            // 
            this.TmPictureBoxImage.Enabled = true;
            this.TmPictureBoxImage.Interval = 1;
            this.TmPictureBoxImage.Tick += new System.EventHandler(this.TmPictureBoxImage_Tick);
            // 
            // btnPLCClear
            // 
            this.btnPLCClear.Location = new System.Drawing.Point(735, 469);
            this.btnPLCClear.Name = "btnPLCClear";
            this.btnPLCClear.Size = new System.Drawing.Size(75, 23);
            this.btnPLCClear.TabIndex = 2;
            this.btnPLCClear.Text = "Clear";
            this.btnPLCClear.UseVisualStyleBackColor = true;
            this.btnPLCClear.Click += new System.EventHandler(this.btnPLCClear_Click);
            // 
            // chCompleteExecute
            // 
            this.chdoNotWaitComplete.AutoSize = true;
            this.chdoNotWaitComplete.Checked = true;
            this.chdoNotWaitComplete.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chdoNotWaitComplete.Location = new System.Drawing.Point(829, 469);
            this.chdoNotWaitComplete.Name = "chCompleteExecute";
            this.chdoNotWaitComplete.Size = new System.Drawing.Size(180, 17);
            this.chdoNotWaitComplete.TabIndex = 3;
            this.chdoNotWaitComplete.Text = "Esegui gli step in modo completo";
            this.chdoNotWaitComplete.UseVisualStyleBackColor = true;
            // 
            // PLCState
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1200, 564);
            this.Controls.Add(this.chdoNotWaitComplete);
            this.Controls.Add(this.btnPLCClear);
            this.Controls.Add(this.pbMove);
            this.Controls.Add(this.richPLCOutput);
            this.Name = "PLCState";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "PLCState";
            ((System.ComponentModel.ISupportInitialize)(this.pbMove)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox richPLCOutput;
        private System.Windows.Forms.PictureBox pbMove;
        private System.Windows.Forms.Timer TmPictureBoxImage;
        private System.Windows.Forms.Button btnPLCClear;
        private System.Windows.Forms.CheckBox chdoNotWaitComplete;
    }
}