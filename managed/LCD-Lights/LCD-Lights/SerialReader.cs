﻿using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;

namespace LCD_Lights
{
	class SerialReader
	{
		private Form1 myForm;
		private SerialPort myPort;
		private bool myfCancelled = false;
		Thread myThread;

		public SerialReader(Form1 theForm)
		{
			myForm = theForm;
		}

		public void Start()
		{
			myThread = new Thread(new ThreadStart(ThreadFunction));
			myfCancelled = false;
			myThread.Start();
		}

		public void Stop()
		{
			myfCancelled = true;
			myThread.Join();
		}

		private void ThreadFunction()
		{
			myPort = new SerialPort("COM3", 9600);
			myPort.Open();

			while (!myfCancelled)
			{
				string serialBuffer = myPort.ReadLine();

				if (serialBuffer.Length != Form1.cLights + 1) // The '\r' character appears at the end.
				{
					continue;
				}

				for (int i = 0; i < Form1.cLights; i++)
				{
					char lightValueChar = serialBuffer[i];
					int lightValue = int.Parse(lightValueChar.ToString());
					myForm.myrgLightValues[i] = lightValue;
				}

				myForm.BeginInvoke(myForm.myUpdateLights);
			}

			myPort.Close();
		}
	}
}
