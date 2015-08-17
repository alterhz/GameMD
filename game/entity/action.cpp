#include "action.h"
#include "debug.h"
using namespace NS_IO;

Vector2 CStandAction::GetCurrentPosition(MSTIME msNow) const
{
	return m_v2CurrentPosition;
}


CRunAction::CRunAction(VtPath &vtPath, double fRunSpeed, MSTIME msNow) 
	: m_vtRunPath(vtPath) 
	, m_fRunSpeed(fRunSpeed)
	, m_msTimeBegin(msNow)
{
}

Vector2 CRunAction::GetCurrentPosition(MSTIME msNow) const
{
	int nPassTime = msNow - m_msTimeBegin;
	
	double fRunDistance = m_fRunSpeed * (nPassTime / 1000.0f);

	Vector2 v2CurrentPosition;

	if (m_vtRunPath.size() < 2)
	{
		LOGError("RunAction路径不应该出现小于2个点。");
		return Vector2(0, 0);
	}
	else
	{
		float fTotalDistance = 0.f;
		VtPath::const_iterator it = m_vtRunPath.begin();
		Vector2 v2Prev = (*it);
		++it;
		for (; it!=m_vtRunPath.end(); ++it)
		{
			const Vector2 &v2End = (*it);
			float fDistance = Vector2::Distance(v2Prev, v2End);
			if (fTotalDistance + fDistance >= fRunDistance)
			{
				// 坐标位于这段上
				float f = fRunDistance - fTotalDistance;
				float fAlpha = f / fDistance;
				Vector2 v2 = Vector2::Lerp(v2Prev, v2End, fAlpha);
				return v2;
			}

			fTotalDistance += fDistance;
			v2Prev = v2End;
		}

		// 距离超过最远距离
		return v2Prev;
	}

	return v2CurrentPosition;
}

Vector2 CDeadAction::GetCurrentPosition( MSTIME msNow ) const
{
	return m_v2CurrentPosition;
}
