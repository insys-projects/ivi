namespace sharpScope
{
    partial class sharpScope
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
            this.waveformGraph1 = new NationalInstruments.UI.WindowsForms.WaveformGraph();
            this.xyCursor1 = new NationalInstruments.UI.XYCursor();
            this.waveformPlot1 = new NationalInstruments.UI.WaveformPlot();
            this.XAxis1 = new NationalInstruments.UI.XAxis();
            this.yAxis1 = new NationalInstruments.UI.YAxis();
            this.waveformGraph2 = new NationalInstruments.UI.WindowsForms.WaveformGraph();
            this.xyCursor2 = new NationalInstruments.UI.XYCursor();
            this.waveformPlot2 = new NationalInstruments.UI.WaveformPlot();
            this.xAxis2 = new NationalInstruments.UI.XAxis();
            this.yAxis2 = new NationalInstruments.UI.YAxis();
            ((System.ComponentModel.ISupportInitialize)(this.waveformGraph1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.xyCursor1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.waveformGraph2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.xyCursor2)).BeginInit();
            this.SuspendLayout();
            // 
            // waveformGraph1
            // 
            this.waveformGraph1.Caption = "Waveform";
            this.waveformGraph1.Cursors.AddRange(new NationalInstruments.UI.XYCursor[] {
            this.xyCursor1});
            this.waveformGraph1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.waveformGraph1.Location = new System.Drawing.Point(12, 12);
            this.waveformGraph1.Name = "waveformGraph1";
            this.waveformGraph1.Plots.AddRange(new NationalInstruments.UI.WaveformPlot[] {
            this.waveformPlot1});
            this.waveformGraph1.Size = new System.Drawing.Size(888, 247);
            this.waveformGraph1.TabIndex = 0;
            this.waveformGraph1.UseColorGenerator = true;
            this.waveformGraph1.XAxes.AddRange(new NationalInstruments.UI.XAxis[] {
            this.XAxis1});
            this.waveformGraph1.YAxes.AddRange(new NationalInstruments.UI.YAxis[] {
            this.yAxis1});
            this.waveformGraph1.ZoomFactor = 1F;
            this.waveformGraph1.BeforeDrawPlot += new NationalInstruments.UI.BeforeDrawXYPlotEventHandler(this.waveformGraph1_BeforeDrawPlot);
            this.waveformGraph1.Paint += new System.Windows.Forms.PaintEventHandler(this.waveformGraph1_Paint);
            // 
            // xyCursor1
            // 
            this.xyCursor1.LabelVisible = true;
            this.xyCursor1.Plot = this.waveformPlot1;
            // 
            // waveformPlot1
            // 
            this.waveformPlot1.XAxis = this.XAxis1;
            this.waveformPlot1.YAxis = this.yAxis1;
            // 
            // XAxis1
            // 
            this.XAxis1.AutoMinorDivisionFrequency = 2;
            this.XAxis1.Caption = "Time";
            this.XAxis1.EditRangeNumericFormatMode = NationalInstruments.UI.NumericFormatMode.CreateSimpleDoubleMode();
            this.XAxis1.MajorDivisions.GridVisible = true;
            this.XAxis1.MajorDivisions.Interval = 10D;
            this.XAxis1.MajorDivisions.LabelFormat = new NationalInstruments.UI.FormatString(NationalInstruments.UI.FormatStringMode.Engineering, "s0");
            this.XAxis1.MinorDivisions.GridLineStyle = NationalInstruments.UI.LineStyle.Dot;
            this.XAxis1.MinorDivisions.GridVisible = true;
            this.XAxis1.Mode = NationalInstruments.UI.AxisMode.AutoScaleVisibleExact;
            this.XAxis1.OriginLineVisible = true;
            // 
            // yAxis1
            // 
            this.yAxis1.MajorDivisions.GridVisible = true;
            this.yAxis1.MajorDivisions.Interval = 10D;
            this.yAxis1.MinorDivisions.GridLineStyle = NationalInstruments.UI.LineStyle.Dot;
            this.yAxis1.MinorDivisions.GridVisible = true;
            // 
            // waveformGraph2
            // 
            this.waveformGraph2.Caption = "Spectrogram";
            this.waveformGraph2.Cursors.AddRange(new NationalInstruments.UI.XYCursor[] {
            this.xyCursor2});
            this.waveformGraph2.Location = new System.Drawing.Point(12, 282);
            this.waveformGraph2.Name = "waveformGraph2";
            this.waveformGraph2.Plots.AddRange(new NationalInstruments.UI.WaveformPlot[] {
            this.waveformPlot2});
            this.waveformGraph2.Size = new System.Drawing.Size(888, 247);
            this.waveformGraph2.TabIndex = 1;
            this.waveformGraph2.UseColorGenerator = true;
            this.waveformGraph2.XAxes.AddRange(new NationalInstruments.UI.XAxis[] {
            this.xAxis2});
            this.waveformGraph2.YAxes.AddRange(new NationalInstruments.UI.YAxis[] {
            this.yAxis2});
            this.waveformGraph2.ZoomFactor = 1F;
            // 
            // xyCursor2
            // 
            this.xyCursor2.LabelVisible = true;
            this.xyCursor2.Plot = this.waveformPlot2;
            // 
            // waveformPlot2
            // 
            this.waveformPlot2.XAxis = this.xAxis2;
            this.waveformPlot2.YAxis = this.yAxis2;
            // 
            // xAxis2
            // 
            this.xAxis2.Caption = "Freq";
            this.xAxis2.MajorDivisions.GridVisible = true;
            this.xAxis2.MajorDivisions.Interval = 10D;
            this.xAxis2.MinorDivisions.GridLineStyle = NationalInstruments.UI.LineStyle.Dot;
            this.xAxis2.MinorDivisions.GridVisible = true;
            // 
            // yAxis2
            // 
            this.yAxis2.MajorDivisions.GridVisible = true;
            this.yAxis2.MajorDivisions.Interval = 10D;
            this.yAxis2.MinorDivisions.GridLineStyle = NationalInstruments.UI.LineStyle.Dot;
            this.yAxis2.MinorDivisions.GridVisible = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(897, 572);
            this.Controls.Add(this.waveformGraph2);
            this.Controls.Add(this.waveformGraph1);
            this.Name = "Form1";
            this.Text = "Scope#";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.waveformGraph1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.xyCursor1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.waveformGraph2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.xyCursor2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private NationalInstruments.UI.WindowsForms.WaveformGraph waveformGraph1;
        private NationalInstruments.UI.WaveformPlot waveformPlot1;
        private NationalInstruments.UI.XAxis XAxis1;
        private NationalInstruments.UI.YAxis yAxis1;
        private NationalInstruments.UI.WindowsForms.WaveformGraph waveformGraph2;
        private NationalInstruments.UI.WaveformPlot waveformPlot2;
        private NationalInstruments.UI.XAxis xAxis2;
        private NationalInstruments.UI.YAxis yAxis2;
        private NationalInstruments.UI.XYCursor xyCursor1;
        private NationalInstruments.UI.XYCursor xyCursor2;
    }
}

