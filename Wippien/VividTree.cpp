#include "stdafx.h"
#include "UserList.h"
#include "VividTree.h"

void CVividTree::DrawItems(CDC *pDC)
{
	// draw items
	HTREEITEM show_item, parent;
	CRect rc_item;
	CString name;
	COLORREF color;
	DWORD tree_style;
	CDC dc_mem;
	int count = 0;
	int state;
	BOOL selected, highlited;
	bool has_children;
	CUser *user = NULL;
	
	
	show_item = GetFirstVisibleItem();
	if (show_item == NULL)
		return;
	
	dc_mem.CreateCompatibleDC(NULL);
	color = pDC->GetTextColor();
	RECT rcowner;
	::GetClientRect(m_hWnd, &rcowner);
	tree_style = ::GetWindowLong(m_hWnd, GWL_STYLE); 
//		POINT ptArc = {8,8};

	EnterCriticalSection(&m_UserCS);
	do
	{
		
//			if (user)
		{

			state = GetItemState(show_item, TVIF_STATE);
			parent = GetParentItem(show_item);
			has_children = ItemHasChildren(show_item) || parent == NULL;
			selected = (state & TVIS_SELECTED) && ((this->m_hWnd == GetFocus()) || (tree_style & TVS_SHOWSELALWAYS));
			highlited = (state & TVIS_DROPHILITED);

			if (GetItemRect(show_item, rc_item, TRUE))
			{
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
					if (!has_children)
						rect.left = rc_item.left + m_h_offset;
					else
						rect.left = m_h_offset;

					user = (CUser *)GetItemData(show_item);
					if (highlited && !user)
					{
						RECT rc;
						memcpy(&rc,&rect, sizeof(rc));
						InflateRect(&rc, -1, -1);
						rc.left += 1;
						Rectangle(pDC->m_hDC, rc.left, rc.top, rc.right, rc.bottom);
					}
					user = (CUser *)GetItemData(show_item);
					if (selected && user)
					{
						// Draw round-rect selection
						RECT rc;
						memcpy(&rc,&rect, sizeof(rc));
						rc.left -= 30;
						pDC->FillSolidRect(&rc, ::GetSysColor(COLOR_WINDOW));
						::InflateRect(&rc, -1, -1);
						CPen pen;
						pen.CreatePen(PS_SOLID, 1, RGB(187,195,215));
//						pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
						CBrush brush;
						brush.CreateSolidBrush(RGB(231,240,254));
//						brush.CreateSolidBrush(RGB(240,240,250));
						HPEN hOldPen = pDC->SelectPen(pen);
						HBRUSH hOldBrush = pDC->SelectBrush(brush);
//						pDC->RoundRect(&rc, ptArc);
						pDC->Rectangle(&rc);
						pDC->SelectBrush(hOldBrush);
						pDC->SelectPen(hOldPen);
//						ATLTRACE("redraw\r\n");
					}



//					GradientFillRect(pDC, rect, from, m_gradient_bkgd_to, FALSE);
					pDC->SetTextColor(RGB(0, 0, 0));
					
					if (has_children)
					{
						// Draw an Open/Close button
						if (state & TVIS_EXPANDED)
							m_TreeOpened.Draw(pDC->m_hDC, rc_item.left - 12, rc_item.top+2);
						else
							m_TreeClosed.Draw(pDC->m_hDC, rc_item.left - 12, rc_item.top+2);
					}
				}
				if (!has_children)
				{
					user = (CUser *)GetItemData(show_item);

					if (_Settings.m_ShowContactPicture)
					{
						if (user && user->m_Image)	
						{
							int h = user->m_Image->GetHeight();
							int h1 = 3;
							if (h< 32)
								h1 = (32 - h)/2;
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

				BSTR t = NULL;
				GetItemText(show_item , t);
				CComBSTR2 t1 = t;
				::SysFreeString(t);
				name = t1.ToString();
				rc_item.DeflateRect(0, 1, 0, 1);
				HFONT hOldFont = pDC->SelectFont(m_hFont);
				if (selected)
				{
//					if (!has_children)
					COLORREF col = pDC->GetBkColor();
					pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
					if (!user)
					{
						RECT rc;
						memcpy(&rc, &rc_item, sizeof(rc));
						pDC->SetTextColor(RGB(0,128,255));
						rc.left += 5;
						pDC->DrawText(name, strlen(name), &rc, DT_LEFT);
					}
					else
					{
						user = (CUser *)GetItemData(show_item);
						if (_Settings.m_ShowContactPicture)
							rc_item.left += 5;
						else
							rc_item.left -= 20;
						RECT rcc;
						memcpy(&rcc, rc_item, sizeof(rcc));
						COLORREF cf = pDC->GetTextColor();
						if (user->m_Block)
							pDC->SetTextColor(RGB(255,32,32));
						pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), &rcc, DT_LEFT | DT_CALCRECT);
						pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), rc_item, DT_LEFT);
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
						if (_Settings.m_ShowContactStatus)
						{
							pDC->DrawText(user->m_SubText, strlen(user->m_SubText), &rc1, DT_LEFT | DT_CALCRECT);
							pDC->DrawText(user->m_SubText, strlen(user->m_SubText), rc_item, DT_LEFT);
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
											pDC->SetTextColor(RGB(255,255,255));
										break;

//									case WipDisconnected:
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
				}
				else
				{
					user = (CUser *)GetItemData(show_item);
					if (!user)
					{
						RECT rc;
						memcpy(&rc, &rc_item, sizeof(rc));
						rc.left += 5;
						pDC->SetTextColor(RGB(0,128,255));
						pDC->DrawText(name, strlen(name), &rc, DT_LEFT);
					}
					else
					{
						if (_Settings.m_ShowContactPicture)
							rc_item.left += 5;
						else
							rc_item.left -= 20;
						RECT rcc;
						memcpy(&rcc, rc_item, sizeof(rcc));
						COLORREF cf = pDC->GetTextColor();
						if (user->m_Block)
							pDC->SetTextColor(RGB(255,32,32));
						pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), &rcc, DT_LEFT | DT_CALCRECT);
						pDC->DrawText(user->m_VisibleName, strlen(user->m_VisibleName), rc_item, DT_LEFT);
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
						if (_Settings.m_ShowContactStatus)
						{
							pDC->DrawText(user->m_SubText, strlen(user->m_SubText), &rc1, DT_LEFT | DT_CALCRECT);
							pDC->DrawText(user->m_SubText, strlen(user->m_SubText), rc_item, DT_LEFT);
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
											pDC->SetTextColor(RGB(255,255,255));
										break;

									//case WipDisconnected:
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
				}
				pDC->SelectFont(hOldFont);
				// if ( state & TVIS_BOLD )
				//	pDC->SelectObject( font );
			}
		}
	} while ((show_item = GetNextVisibleItem(show_item)) != NULL);
	LeaveCriticalSection(&m_UserCS);
}
