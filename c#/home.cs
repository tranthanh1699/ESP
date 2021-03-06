using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json; 

namespace MPS_ADMIN
{
    public partial class HomeScreen : Form
    {

        static int userPanelCount = 0;
        
        public HomeScreen()
        {
            InitializeComponent();
        }
        Random R = new Random();
        private async void button1_Click(object sender, EventArgs e)
        {
            list_pn.Controls.Clear(); 
            for(int i = 1; i <= 5; i++)
            {
                userAddPannel("Hello"); 
            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            list_pn.Controls.Clear(); 
            userPanelCount = 0;
            list_pn.AutoScroll = false; 
        }

        private async Task<string> getJsonHome(string email, int document)
        {
            String JsonData = await FireBaseApplication.getUserData(email, document); 
            return JsonData; 
        }
        private void userAddPannel(string userContent)
        {
            Panel p = new Panel();
            p.Name = "panel" + (list_pn.Controls.Count + 1);
            p.BackColor = Color.FromArgb(157, 251, 186);
            p.Size = new Size(list_pn.ClientSize.Width, 50);
            p.Padding = new Padding(50);
            p.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle; 
            Label addLable = new Label();
            addLable.Size = new Size(p.Width, p.Height);


            addLable.Text = userContent; 
            p.Controls.Add(addLable); 
            list_pn.Controls.Add(p);
            p.Dock = DockStyle.Top;
            
            userPanelCount++;
            if (userPanelCount > 10)
            {
                list_pn.AutoScroll = true;
            }

            else list_pn.AutoScroll = false;
        }
    }
    public class Message
    {
        public int flow { get; set; }
        public int s { get; set; }
        public int speed { get; set; }
    }

    public class userDetailPanel : Panel
    {
        userDetailPanel()
        {
            this.Size = new Size(50, 500);
            this.BackColor = System.Drawing.Color.FromArgb(112, 221, 1);
            this.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D; 
        }
    }

}
