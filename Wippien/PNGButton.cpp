#include "stdafx.h"
#include <atlcrack.h>
#include "PNGButton.h"
#include "BaloonHelp.h"

LRESULT CPNGButton::OnTimer2(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 103)
	{
		::KillTimer(m_hWnd, 103);
	
		m_pBalloon = new CBalloonHelp;
		POINT p;
		RECT rc;
		GetWindowRect(&rc);
		p.x = m_TooltipPoint.x + rc.left;
		p.y = m_TooltipPoint.y + rc.top;

		m_pBalloon->Create(m_hWnd, NULL, m_ToolTip, NULL, &p,	
			CBalloonHelp::BallonOptions::BOCloseOnButtonDown | 
			CBalloonHelp::BallonOptions::BOCloseOnButtonUp | 
			CBalloonHelp::BallonOptions::BOCloseOnMouseMove | 
			CBalloonHelp::BallonOptions::BOCloseOnKeyDown | 
			CBalloonHelp::BallonOptions::BOCloseOnAppDeactivate | 
			CBalloonHelp::BallonOptions::BODisableFadeOut | 
			CBalloonHelp::BallonOptions::BOShowTopMost | 
			/*CBalloonHelp::BallonOptions::BODeleteThisOnClose, // delete pBalloon on close */
			NULL,
			0);

//		memset(&m_TooltipPoint, 0, sizeof(m_TooltipPoint));
	}
		
	return TRUE;
}

LRESULT CPNGButton::OnMouseLeave(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::KillTimer(m_hWnd, 103);

	if (m_pBalloon->IsWindow())
		m_pBalloon->DestroyWindow();
	delete m_pBalloon;
	m_pBalloon = NULL;
	memset(&m_TooltipPoint, 0, sizeof(m_TooltipPoint));

	return TRUE;
}

LRESULT CPNGButton::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_ToolTip)
	{
		POINT p;
		p.x = GET_X_LPARAM(lParam); 
		p.y = GET_Y_LPARAM(lParam); 	
		if (p.x != m_TooltipPoint.x || p.y != m_TooltipPoint.y)
		{
			if (!m_pBalloon)
			{
				m_TooltipPoint.x = p.x;
				m_TooltipPoint.y = p.y;
				::SetTimer(m_hWnd, 103, 500, NULL); // TOOLTIP timer

			}
			else
			{
				if (m_pBalloon->IsWindow())
					m_pBalloon->DestroyWindow();
				delete m_pBalloon;
				m_pBalloon = NULL;
			}
		}
	}

	return TRUE;
}
