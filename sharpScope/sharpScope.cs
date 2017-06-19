using NationalInstruments.Analysis;
using NationalInstruments.Analysis.Conversion;
using NationalInstruments.Analysis.Dsp;
using NationalInstruments.Analysis.Dsp.Filters;
using NationalInstruments.Analysis.Math;
using NationalInstruments.Analysis.Monitoring;
using NationalInstruments.Analysis.SignalGeneration;
using NationalInstruments.Analysis.SpectralMeasurements;
using NationalInstruments;
using NationalInstruments.UI;
using NationalInstruments.DAQmx;
using NationalInstruments.NI4882;
using NationalInstruments.NetworkVariable;
using NationalInstruments.NetworkVariable.WindowsForms;
using NationalInstruments.Tdms;
using NationalInstruments.UI.WindowsForms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


namespace sharpScope
{
    public partial class sharpScope : Form
    {
        double rateX;
        int actualPts;
        int actualRecordLength;
        double initialX;
        double incrementX;
        double[] waveform;// = new double[actualRecordLength * 2];
        double[] autoPowerSpectrum;
        itScope scope;

        private class XCustomFormatString : FormatString
        {
            private double scalingFactor;
            public XCustomFormatString(double scaling)
                : base(FormatStringMode.Engineering,"S")
            {
                scalingFactor = scaling;
            }
            public override string FormatDouble(double value)
            {
                return base.FormatDouble(value * scalingFactor);
            }
        }

        private class YLogCustomFormatString : FormatString
        {
            private double scalingFactor;
            public YLogCustomFormatString(double scaling)
                : base(FormatStringMode.Engineering, "S")
            {
                scalingFactor = scaling;
            }
            public override string FormatDouble(double value)
            {
                value = 10.0 * Math.Log10(value*scalingFactor);
                return base.FormatDouble(value);
            }
        }

        public sharpScope()
        {
            InitializeComponent();
            scope = new itScope("pe510k1", true, true);
            scope.ConfigureAcquisitionType(itScopeConstants.Normal);
            scope.ConfigureAcquisitionRecord(0.000005, 4096, 0.0);
            scope.ConfigureChannel("CH1", 2.0, 0.0, itScopeConstants.Dc, 1.0, true);
            scope.ConfigureChanCharacteristics("CH1", 50, 200000000);
            scope.SampleRate(out rateX);
            scope.ActualRecordLength(out actualRecordLength);
            waveform = new double[actualRecordLength];

            waveformPlot1.XAxis.Range = new NationalInstruments.UI.Range(0, actualRecordLength * (1.0 / rateX));
            waveformPlot1.XAxis.MajorDivisions.LabelFormat = new XCustomFormatString(1.0 / rateX);

            waveformPlot2.XAxis.Range = new NationalInstruments.UI.Range(0, rateX/2.0);
            waveformPlot2.XAxis.MajorDivisions.LabelFormat = new XCustomFormatString(rateX / (actualRecordLength));

           // waveformPlot2.YAxis.MajorDivisions.LabelFormat = new YLogCustomFormatString((double)actualRecordLength);

        }

        private void waveformGraph1_BeforeDrawPlot(object sender, BeforeDrawXYPlotEventArgs e)
        {
            scope.ReadWaveform("CH1", actualRecordLength, 5000, waveform, out actualPts, out initialX, out incrementX);
        }

        private void waveformGraph1_Paint(object sender, PaintEventArgs e)
        {
            waveformPlot1.PlotY(waveform);

            // Calculate the single-sided, scaled auto power spectrum of signal
            // Set the sample period
            double dt, df;
            dt = 1.0 / (actualPts);
            //UnscaledWindow.Blackman(waveform);
            //UnscaledWindow.BlackmanHarris(waveform);
            //UnscaledWindow.ExactBlackman(waveform);
            //UnscaledWindow.FlatTop(waveform);
            //UnscaledWindow.Hamming(waveform);
            UnscaledWindow.Hanning(waveform);

            autoPowerSpectrum = Measurements.AutoPowerSpectrum(waveform, dt, out df);

            double maximum = ArrayOperation.GetMax(autoPowerSpectrum);
            //maximum = Math.Sqrt(maximum);

            for(int i=0;i<actualPts/2;i++)
            {
                autoPowerSpectrum[i] = 10.0*Math.Log10(Math.Max(autoPowerSpectrum[i]/maximum,1.0e-15)); 
            }

            waveformPlot2.PlotY(autoPowerSpectrum);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
