using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ClientServerTCP
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        string ServerIp,Mesaj;
        int ClientPort;
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            ServerIp = txtServerIp.Text;
            ClientPort = Convert.ToInt32(txtPort.Text);
            Mesaj = txtMesaj.Text;
             try
        {
            // TCP istemcisi oluştur
            TcpClient client = new TcpClient();
            txtPano.AppendText("\r\n" + "Sunucuya bağlanılıyor...");

            // Sunucuya bağlan
            client.Connect(ServerIp, ClientPort);          
            txtPano.AppendText("\r\n" + "Sunucuya bağlandı!");

            // Sunucuya mesaj gönder
            NetworkStream stream = client.GetStream();
            string message = txtMesaj.Text;
            byte[] dataToSend = Encoding.UTF8.GetBytes(message);
            stream.Write(dataToSend, 0, dataToSend.Length);
            txtPano.AppendText("\r\n" + "Sunucuya gönderilen mesaj:"+ message);

            // Sunucudan yanıt al
            byte[] receivedData = new byte[1024];
            int bytesReceived = stream.Read(receivedData, 0, receivedData.Length);
            string response = Encoding.UTF8.GetString(receivedData, 0, bytesReceived);
            txtPano.AppendText("\r\n" + "Sunucudan gelen yanıt: "+response);

            // Bağlantıyı kapat
            stream.Close();
            client.Close();
            txtPano.AppendText("\r\n" + "Bağlantı kapatıldı.");
        }
        catch (Exception ex)
        {
            txtPano.AppendText("\r\n" + "Hata:"+ ex.Message);
        }
        }
    }
}
