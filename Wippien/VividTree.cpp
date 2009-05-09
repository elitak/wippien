#include "stdafx.h"
#include "UserList.h"
#include "VividTree.h"
#include "Jabber.h"

extern CJabber *_Jabber;


void CVividTree::DrawItems(CDC *pDC)
{
	unsigned long tick = GetTickCount();
	// draw items
	HTREEITEM show_item, parent;
	CRect rc_item;
	COLORREF color;
	DWORD tree_style;
	CDC dc_mem;
	int count = 0;
	int state;
	BOOL selected, highlited;
	bool has_children;
	CUser *user = NULL;
	char LastReceiveBuff[4] = "<>\0";
	char name[1024];
	
	show_item = GetFirstVisibleItem();
	if (show_item == NULL)
		return;
	
	dc_mem.CreateCompatibleDC(NULL);
	color = pDC->GetTextColor();
	RECT rcowner;
	::GetClientRect(m_hWnd, &rcowner);
	tree_style = ::GetWindowLong(m_hWnd, GWL_STYLE); 

	CPen groupline; groupline.CreatePen(PS_SOLID, 1, RGB(192,192,192));
	CBrush brush;brush.CreateSolidBrush(RGB(231,240,254));
	CPen pen;pen.CreatePen(PS_SOLID, 1, RGB(187,195,215));

//	ATLTRACE("Invalidate %lu\r\n", GetTickCount());
	EnterCriticalSection(&m_UserCS);
	do
	{
		CSettings::TreeGroup *tg = NULL;	
		{

			state = GetItemState(show_item, TVIF_STATE);
			parent = GetParentItem(show_item);
			has_children = ItemHasChildren(show_item) || parent == NULL;
			selected = (state & TVIS_SELECTED) && ((this->m_hWnd == GetFocus()) || (tree_style & TVS_SHOWSELALWAYS));
			highlited = (state & TVIS_DROPHILITED);

			name[0] = 0;
			tg = NULL;
			TVITEMEX lvitem = { 0 };
			lvitem.hItem = show_item;
			lvitem.pszText = name;
			lvitem.cchTextMax = sizeof(name);
			lvitem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_INTEGRAL;
			
			::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&lvitem);
			user = (CUser *)lvitem.lParam;
			if (!user)
			{
				tg = _Settings.GetGroupByName(name);
				if (tg && tg->VisibleName)
					strcpy(name, tg->VisibleName);
			}

			if (GetItemRect(show_item, rc_item, TRUE))
			{
				if (rc_item.bottom - rc_item.top < lvitem.iIntegral)
					rc_item.bottom = rc_item.top + lvitem.iIntegral;

				rc_item.right = rcowner.right/*-rcowner.left+rc_item.left*/;
				if (has_children  || selected || highlited)
				{
					COLORREF from;
					CRect rect;
					// Show 
					if (selected)
						from = m_gradient_bkgd_sel;
					else
						from = m_gradient_bkgd_to - (m_gradient_bkgd_from - m_gradient_bkgd_to);
					rect.top = rc_item.top;
					rect.bottom = rc_item.bottom;
					rect.right = m_h_size + m_h_offset;
					//if (!has_children)
						rect.left = rc_item.left + m_h_offset;
					//else
					//	rect.left = m_h_offset;

					if (highlited && !user)
					{
						RECT rc;
						memcpy(&rc,&rect, sizeof(rc));
						InflateRect(&rc, -1, -1);
						rc.left -= 16;
						rc.top += 9;
						Rectangle(pDC->m_hDC, rc.left, rc.top, rc.right, rc.bottom);
					}
					if (selected && user)
					{
						// Draw round-rect selection
						RECT rc;
						memcpy(&rc,&rect, sizeof(rc));

						if (_Settings.m_ShowContactPicture && !user->m_ChatRoomPtr)
							rc.left += 4;
						else
							rc.left -= 22;
						rc.top += 1;
						rc.bottom += 1;
						pDC->FillSolidRect(&rc, ::GetSysColor(COLOR_WINDOW));
						::InflateRect(&rc, -1, -1);
						HPEN hOldPen = pDC->SelectPen(pen);
						HBRUSH hOldBrush = pDC->SelectBrush(brush);
						pDC->Rectangle(&rc);
						pDC->SelectBrush(hOldBrush);
						pDC->SelectPen(hOldPen);
					}



					pDC->SetTextColor(RGB(0, 0, 0));
					
					if (has_children)
					{
						// Draw an Open/Close button
						if (state & TVIS_EXPANDED)
							m_GroupOpened.Draw(pDC->m_hDC, rc_item.left - 14, rc_item.top+15);
						else
							m_GroupClosed.Draw(pDC->m_hDC, rc_item.left - 14, rc_item.top+15);
					}
				}
				if (!has_children)
				{
					if (_Settings.m_ShowContactPicture && !user->m_ChatRoomPtr)
					{
						if (user && user->m_Image)	
						{
							int h = user->m_Image->GetHeight();
							int h1 = 3;
							if (h< 34)
								h1 = (34 - h)/2;

							//ATLTRACE("Drawing image for %s, away is %d\r\n", user->m_JID, user->m_IsAway);
							if (user->m_IsAway)
							{
								CxImage *d = new CxImage();
								d->Copy(*user->m_Image);
								d->Light(50,-40);
								d->Draw(pDC->m_hDC, rc_item.left-28, rc_item.top+h1);
								delete d;
							}
							else
								user->m_Image->Draw(pDC->m_hDC, rc_item.left-28, rc_item.top+h1);
						}
					}

					// draw blink
					if (user && user->m_BlinkTimerCounter && user->m_BlinkTimerCounter%2)
					{
						m_BlinkImage.Draw(pDC->m_hDC, rc_item.right - 20, rc_item.top + 12);
					}

				}
				rc_item.left ++;

				rc_item.DeflateRect(0, 1, 0, 1);
				HFONT hOldFont = pDC->SelectFont(m_hFont);

				COLORREF col = pDC->GetBkColor();
				if (selected)
					pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));

				if (!user)
				{
					if (tg)
					{
						RECT rc;
						memcpy(&rc, &rc_item, sizeof(rc));
						pDC->SetTextColor(RGB(0,128,255));
						rc.left -= 8;
						rc.top += 10;
						HFONT oldfont = pDC->SelectFont(m_hGroupFont);
						pDC->DrawText(name, strlen(name), &rc, DT_LEFT);
						if (tg->Block)
						{
							RECT rc2 = {0};
							pDC->DrawText(name, strlen(name), &rc2, DT_LEFT | DT_CALCRECT);
							m_LockContact.Draw(pDC->m_hDC, rc.left+rc2.right+2, rc.top+4);
						}
						pDC->SelectFont(m_hSubFont);
						rc.right -= 10;
						rc.top += 5;
						pDC->SetTextColor(RGB(128,128,128));
						pDC->DrawText(tg->CountBuff, strlen(tg->CountBuff), &rc, DT_RIGHT);
						pDC->SelectFont(oldfont);
						HPEN hOldPen = pDC->SelectPen(groupline);
						pDC->MoveTo(rc_item.left-15, rc_item.bottom-2);
						pDC->LineTo(rc_item.right-3, rc_item.bottom-2);
						pDC->SelectPen(hOldPen);
					}
				}
				else
				{
					if (_Settings.m_ShowContactPicture && !user->m_ChatRoomPtr)
						rc_item.left += 5;
					else
						rc_item.left -= 20;
					RECT rcc;
					memcpy(&rcc, rc_item, sizeof(rcc));
					COLORREF cf;
					if (selected)
						cf = RGB(0,0,0);
					else
						cf = RGB(96,96,96);

					pDC->SetTextColor(RGB(0,0,0));
					::DrawTextW(pDC->m_hDC, user->m_bstrVisibleName, user->m_bstrVisibleName.Length(), &rcc, DT_LEFT | DT_CALCRECT);
					//pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), &rcc, DT_LEFT | DT_CALCRECT);
					if (_Settings.m_ShowContactActivity)
					{
						int l = rc_item.left;
						// show activity
						if (_Settings.m_ShowContactPicture)
							rc_item.top += 20;
						else
							rc_item.left = rcc.right + 2;

						if (user->m_LastReceive + 500 > tick)
							LastReceiveBuff[1] = '.';
						else
							LastReceiveBuff[1] = ' ';
						if (user->m_LastSent + 500 > tick)
							LastReceiveBuff[0] = '.';
						else
							LastReceiveBuff[0] = ' ';
						pDC->DrawText(LastReceiveBuff, 2, rc_item, DT_LEFT);
						if (_Settings.m_ShowContactPicture)
						{
							rc_item.top -= 20;
						}
						else
						{
							rc_item.left = l;
						}

					}


					::DrawTextW(pDC->m_hDC, user->m_bstrVisibleName, user->m_bstrVisibleName.Length(), rc_item, DT_LEFT);
					//pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), rc_item, DT_LEFT);
					if (user->m_Block)
					{
						RECT rc2 = {0};
						::DrawTextW(pDC->m_hDC, user->m_bstrVisibleName, user->m_bstrVisibleName.Length(), &rc2, DT_LEFT | DT_CALCRECT);
//						pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), &rc2, DT_LEFT | DT_CALCRECT);
//						rc2.left += rc_item.left;
//						rc2.top += rc_item.top;
						m_LockContact.Draw(pDC->m_hDC, rc_item.left+rc2.right+2, rc_item.top+2);

//						pDC->SetTextColor(RGB(255,32,32));
					}
//					else

					if (!selected)
						pDC->SetTextColor(RGB(56,56,56));

					::DrawTextW(pDC->m_hDC, user->m_bstrVisibleName, user->m_bstrVisibleName.Length(), rc_item, DT_LEFT);
//					pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), rc_item, DT_LEFT);
					if (!user->m_Online && user->m_WippienState==WipConnected)
						pDC->SetTextColor(RGB(127,127,127));
					else
						pDC->SetTextColor(cf);
					rc_item.top += 12;
					rc_item.left += 3;
					pDC->SelectFont(m_hSubFont);
					RECT rc1;
					rc_item.right-=2;
					memcpy(&rc1,&rc_item, sizeof(RECT));
					if (_Settings.m_ShowContactStatus && !user->m_ChatRoomPtr)
					{
//						pDC->DrawText(user->m_SubText, strlen(user->m_SubText), &rc1, DT_LEFT | DT_CALCRECT);
//						pDC->DrawText(user->m_SubText, strlen(user->m_SubText), rc_item, DT_LEFT);
						::DrawTextW(pDC->m_hDC, user->m_bstrSubText, user->m_bstrSubText.Length(), &rc1, DT_LEFT | DT_CALCRECT);
						::DrawTextW(pDC->m_hDC, user->m_bstrSubText, user->m_bstrSubText.Length(), rc_item, DT_LEFT);
					}
					if (_Settings.m_ShowContactIP)
					{
						if (user->m_IPText[0])
						{
							rc_item.top -= 10;
							rc_item.right -= 3;
							switch (user->m_WippienState)
							{
								case WipConnected:
									if (user->m_Online)
										pDC->SetTextColor(RGB(0,0,0));
									else
										pDC->SetTextColor(RGB(127,127,127));
									break;

								case WipConnecting:
									if (user->m_BlinkConnectingCounter%2)
										pDC->SetTextColor(RGB(127,127,255));
									else
										pDC->SetTextColor(RGB(64,64,192));
									break;

								default:
									pDC->SetTextColor(RGB(127,127,255));
									break;

							}
							rc_item.left = rcc.right + 10;
							// let's calculate clipping
							memcpy(&rcc, &rc_item, sizeof(RECT));

							pDC->DrawText(user->m_IPText, strlen(user->m_IPText), &rcc, DT_RIGHT | DT_CALCRECT);
							if (rcc.right > rc_item.right)
							{
								pDC->DrawText("(...", 4, rc_item, DT_LEFT);
								rc_item.left += 10;
							}
							pDC->DrawText(user->m_IPText, strlen(user->m_IPText), rc_item, DT_RIGHT);
						}
					}
				}
				pDC->SetTextColor(color);
				pDC->SetBkColor(col);
				pDC->SelectFont(hOldFont);
				// if ( state & TVIS_BOLD )
				//	pDC->SelectObject( font );
			}
		}
	} while ((show_item = GetNextVisibleItem(show_item)) != NULL);
	LeaveCriticalSection(&m_UserCS);
	DeleteObject(groupline);
}

#if _OWNER_DRAWN
LRESULT CVividTree::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CPaintDC dc(m_hWnd);	// Device context for painting
	CDC dc_ff;			// Memory base device context for flicker free painting
	CBitmap bm_ff;		// The bitmap we paint into
	HBITMAP bm_old;
	//HFONT font, old_font;
	//CFont fontDC;
	int old_mode;
	
	GetClientRect(&m_rect);
	SCROLLINFO scroll_info;
	// Determine window portal to draw into taking into account
	// scrolling position
	scroll_info.fMask = SIF_POS | SIF_RANGE;
	if (GetScrollInfo(SB_HORZ, &scroll_info/*, SIF_POS | SIF_RANGE*/))
	{
		m_h_offset = -scroll_info.nPos;
		m_h_size = max(scroll_info.nMax + 1, m_rect.Width());
	}
	else
	{
		m_h_offset = m_rect.left;
		m_h_size = m_rect.Width();
	}
	scroll_info.fMask = SIF_POS | SIF_RANGE;
	if (GetScrollInfo(SB_VERT, &scroll_info/*, SIF_POS | SIF_RANGE */))
	{
		if (scroll_info.nMin == 0 && scroll_info.nMax == 100) 
			scroll_info.nMax = 0;
		m_v_offset = -scroll_info.nPos * GetItemHeight();
		m_v_size = max((scroll_info.nMax + 2)* ((int)GetItemHeight() + 1), m_rect.Height());
	}
	else
	{
		m_v_offset = m_rect.top;
		m_v_size = m_rect.Height();
	}
	
	// Create an offscreen dc to paint with (prevents flicker issues)
	dc_ff.CreateCompatibleDC(dc);
	bm_ff.CreateCompatibleBitmap(dc, m_rect.Width(), m_rect.Height());
	// Select the bitmap into the off-screen DC.
	bm_old = dc_ff.SelectBitmap(bm_ff);
	// Default font in the DC is not the font used by 
	// the tree control, so grab it and select it in.
	//font = GetFont();
	//old_font = dc_ff.SelectFont(font);
	// We're going to draw text transparently
	old_mode = dc_ff.SetBkMode(TRANSPARENT);
	
	DrawBackGround(&dc_ff);
	DrawItems(&dc_ff);
	
	// Now Blt the changes to the real device context - this prevents flicker.
	dc.BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), dc_ff, 0, 0, SRCCOPY);
	
	//dc_ff.SelectFont(old_font);
	dc_ff.SetBkMode(old_mode);
	dc_ff.SelectBitmap(bm_old);
	
	return TRUE;
}
#endif