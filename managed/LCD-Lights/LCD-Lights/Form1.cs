using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace LCD_Lights
{
	public partial class Form1 : Form
	{
		public delegate void UpdateLightsDelegate();
		public const int cLights = 7;
		private const int lightStep = 28; // 255/9

		public UpdateLightsDelegate myUpdateLights;
		private Panel[] myrgpnlLights = new Panel[cLights];
		public int[] myrgLightValues = new int[cLights];
		private SerialReader myReader;

		public Form1()
		{
			InitializeComponent();
			myUpdateLights = new UpdateLightsDelegate(SetLightColors);
			myReader = new SerialReader(this);
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			// Initialize all of the panels and add them to the form
			for (int iLight = 0; iLight < myrgpnlLights.Length; iLight++)
			{
				myrgpnlLights[iLight] = new Panel();
				Controls.Add(myrgpnlLights[iLight]);
				myrgLightValues[iLight] = iLight;
			}

			ResizeLights();
			SetLightColors();
			myReader.Start();
		}

		private void Form1_ResizeEnd(object sender, EventArgs e)
		{
			ResizeLights();
		}

		private void ResizeLights()
		{
			int increment = this.Width / 7;
			int left = 0;

			for (int iLight = 0; iLight < myrgpnlLights.Length; iLight++)
			{
				myrgpnlLights[iLight].Left = left;
				myrgpnlLights[iLight].Top = 0;
				myrgpnlLights[iLight].Width = increment;
				myrgpnlLights[iLight].Height = this.Height;

				left += increment;
			}
		}

		private void SetLightColors()
		{
			for (int iLight = 0; iLight < myrgpnlLights.Length; iLight++)
			{
				int brightness = myrgLightValues[iLight] * lightStep;

				myrgpnlLights[iLight].BackColor = Color.FromArgb(brightness, brightness, brightness);
			}
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			myReader.Stop();
		}

	}
}
