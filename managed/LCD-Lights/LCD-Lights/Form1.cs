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
		private const int cLights = 7;
		private const int lightStep = 28; // 255/9

		public UpdateLightsDelegate UpdateLights;
		private Panel[] pnlLights = new Panel[cLights];
		private int[] lightValue = new int[cLights];

		public Form1()
		{
			InitializeComponent();
			UpdateLights = new UpdateLightsDelegate(SetLightColors);
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			// Initialize all of the panels and add them to the form
			for (int iLight = 0; iLight < pnlLights.Length; iLight++)
			{
				pnlLights[iLight] = new Panel();
				Controls.Add(pnlLights[iLight]);
				lightValue[iLight] = iLight;
			}

			ResizeLights();
			SetLightColors();
		}

		private void Form1_ResizeEnd(object sender, EventArgs e)
		{
			ResizeLights();
		}

		private void ResizeLights()
		{
			int increment = this.Width / 7;
			int left = 0;

			for (int iLight = 0; iLight < pnlLights.Length; iLight++)
			{
				pnlLights[iLight].Left = left;
				pnlLights[iLight].Top = 0;
				pnlLights[iLight].Width = increment;
				pnlLights[iLight].Height = this.Height;

				left += increment;
			}
		}

		private void SetLightColors()
		{
			for (int iLight = 0; iLight < pnlLights.Length; iLight++)
			{
				int brightness = lightValue[iLight] * lightStep;

				pnlLights[iLight].BackColor = Color.FromArgb(brightness, brightness, brightness);
			}
		}

	}
}
