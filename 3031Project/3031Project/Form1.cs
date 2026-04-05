using System.Diagnostics;
using System.IO.Ports;

namespace _3031Project
{
    public partial class Form1 : Form
    {


        private SerialPort _serialPort;

        public Form1()
        {
            InitializeComponent();

            string[] ports = SerialPort.GetPortNames();
            comboBox1.DataSource = ports;

            this.KeyPreview = true;
            this.KeyDown += Form1_KeyDown;

        }

        private void SendCommand(string command)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.WriteLine(command);
                Debug.WriteLine("command sent");
            }
        }

        private void startbtn_Click(object sender, EventArgs e)
        {


            string currentPort = Convert.ToString(comboBox1.SelectedItem);
            MessageBox.Show(currentPort);
            _serialPort = new SerialPort(currentPort, 115200, Parity.None, 8, StopBits.One);

            _serialPort.DataReceived += sp_DataRx;
            //_serialPort.Open();
            try
            {
                _serialPort.Open();
            }
            catch (IOException)
            {
                MessageBox.Show("Serial Port Timed Out");
            }

        }

        private void stopbtn_Click(object sender, EventArgs e)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
            }
        }

        private void sp_DataRx(object sender, SerialDataReceivedEventArgs e)
        {


            string data = _serialPort.ReadLine();
            //MessageBox.Show(data);

            // Trim whitespace and check if data is valid
            data = data.Trim();
            if (string.IsNullOrEmpty(data))
                return;

            // in the video, he used ReadLine in VS, and Serial.println in arduino

            m1vbx.Invoke(new Action(() =>
            {
                string[] subs = data.Split(':');

                switch (subs[0])
                {
                    case "m1p":
                        m1pbx.Text = subs[1] + " deg";
                        break;
                    case "m1v":
                        m1vbx.Text = subs[1] + " rad/sec";
                        break;
                    case "m2p":
                        m2pbx.Text = subs[1] + " deg";
                        break;
                    case "m2v":
                        m2vbx.Text = subs[1] + " rad/sec";
                        break;
                    case "m3p":
                        m3pbx.Text = subs[1] + " deg";
                        break;
                    case "m3v":
                        m3vbx.Text = subs[1] + " rad/sec";
                        break;
                    default:
                        MessageBox.Show("Invalid Data Recived" + data);
                        break;
                }
            }));
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Up:
                    SendCommand("grs:inc");
                    break;
                case Keys.Down:
                    SendCommand("grs:dec");
                    break;
                case Keys.Left:
                    SendCommand("gts:inc");
                    break;
                case Keys.Right:
                    SendCommand("gts:dec");
                    break;
                case Keys.PageUp:
                    SendCommand("ghs:inc");
                    break;
                case Keys.PageDown:
                    SendCommand("ghs:dec");
                    break;

            }

        }
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
            }

        }

        private void diagBtn_Click(object sender, EventArgs e)
        {
            SendCommand("cmd:diag");
            //use serial.readstring on the arduino
        }

        private void m1fBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m1s:fwd");
        }

        private void m1rBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m1s:rev");
        }

        private void m2fBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m2s:fwd");
        }

        private void m2rBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m2s:rev");
        }

        private void m3fBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m3s:fwd");
        }

        private void m3vBtn_Click(object sender, EventArgs e)
        {
            SendCommand("m3s:rev");
        }

        private void obstBtn_Click(object sender, EventArgs e)
        {
            SendCommand("cmd:obst");
        }

        private void homeBtn_Click(object sender, EventArgs e)
        {
            SendCommand("cmd:home");
        }

        private void dp1Bx_DoubleClick(object sender, EventArgs e)
        {

            SendCommand("m1a:" + dp1Bx.Text);
        }

        private void dp2Bx_DoubleClick(object sender, EventArgs e)
        {
            SendCommand("m2a:" + dp2Bx.Text);
        }

        private void dp3Bx_DoubleClick(object sender, EventArgs e)
        {
            SendCommand("m3a:" + dp3Bx.Text);
        }
    }
}
