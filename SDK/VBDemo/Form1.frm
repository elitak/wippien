VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command4 
      Caption         =   "SetStatusText"
      Height          =   435
      Left            =   3120
      TabIndex        =   4
      Top             =   840
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   315
      Left            =   1560
      TabIndex        =   3
      Text            =   "Text1"
      Top             =   960
      Width           =   1455
   End
   Begin VB.CommandButton Command3 
      Caption         =   "GetStatusText"
      Height          =   435
      Left            =   180
      TabIndex        =   2
      Top             =   840
      Width           =   1335
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Sound off"
      Height          =   435
      Left            =   1080
      TabIndex        =   1
      Top             =   300
      Width           =   855
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Sound on"
      Height          =   435
      Left            =   180
      TabIndex        =   0
      Top             =   300
      Width           =   855
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim WithEvents WipSDK As WippienSDK
Attribute WipSDK.VB_VarHelpID = -1

Private Sub Command1_Click()
    WipSDK.Sound = True
End Sub

Private Sub Command2_Click()
    WipSDK.Sound = False
End Sub

Private Sub Command3_Click()
    Text1.Text = WipSDK.StatusText
End Sub

Private Sub Command4_Click()
    WipSDK.StatusText = Text1.Text
End Sub

Private Sub Form_Load()
    Set WipSDK = New WippienSDK
    
'    WipSDK.Sound = False
    'WipSDK.CreateAccount "kreso2@wippien.com", "test124", "wippien.com", 5222
End Sub

Private Sub WipSDK_AccountCreated(ByVal ErrorText As String)
    MsgBox "AuthCreated: " & ErrorText
End Sub

Private Sub WipSDK_AuthRequest(ByVal JID As String)
    MsgBox "AuthRequest from " & JID
End Sub

Private Sub WipSDK_Die()
    MsgBox "DIED!"
End Sub

Private Sub WipSDK_UserPresenceChange(ByVal JID As String, ByVal NewPresence As Long, ByVal OldPresence As Long)
    Debug.Print JID & " " & NewPresence
End Sub
