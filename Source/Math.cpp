#include "Math.h"

float DotProduct( const Vector& a, const Vector& b )
{
	return( a.x * b.x + a.y * b.y + a.z * b.z );
}
float DotProduct( const float *v1, const float *v2 )
{
	return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
}
float DegToRad( float alpha )
{
	return alpha / 180.0f * 3.1415926535f;
}
bool InHitbox( Vector vView, Vector vStart, Vector vEnd )
{
	if( ( vView[ 0 ] > vStart[ 0 ] && vView[ 0 ] < vEnd[ 0 ] ) && ( vView[ 1 ] > vStart[ 1 ] && vView[ 1 ] < vEnd[ 1 ] ) ) return true;
	else return false;
}
bool WorldToRadar( int Position[ 2 ], int Size, float ViewAnglesYaw, Vector LocalPosition, Vector EntityPosition, int Range, float *Output )
{
	float flMiddle[ 2 ]			= { ( float )Position[ 0 ] + ( ( float )Size / 2.f ),
									( float )Position[ 1 ] + ( ( float )Size / 2.f ) };

	float flViewAnglesYaw[ 2 ]	= { cos( DegToRad( ViewAnglesYaw ) ),
									sin( DegToRad( ViewAnglesYaw ) ) };

	float flMapPoint[ 2 ]		= { EntityPosition.x - LocalPosition.x,
									EntityPosition.y - LocalPosition.y };

	float flTemp[ 2 ]			= { ( flMapPoint[ 1 ] * flViewAnglesYaw[ 0 ] - flMapPoint[ 0 ] * flViewAnglesYaw[ 1 ] ) / Range,
									( flMapPoint[ 0 ] * flViewAnglesYaw[ 0 ] + flMapPoint[ 1 ] * flViewAnglesYaw[ 1 ] ) / Range };

	Output[ 0 ] =  flTemp[ 0 ] + flMiddle[ 0 ];
	Output[ 1 ] = -flTemp[ 1 ] + flMiddle[ 1 ];

	if( Output[ 0 ] < flMiddle[ 0 ] )
	{
		float flTmp = flMiddle[ 0 ] - Output[ 0 ];
		Output[ 0 ] = flMiddle[ 0 ] + flTmp;
	}
	else if( Output[ 0 ] > flMiddle[ 0 ] )
	{
		float flTmp = Output[ 0 ] - flMiddle[ 0 ];
		Output[ 0 ] = flMiddle[ 0 ] - flTmp;
	}

	if( Output[ 0 ] > Position[ 0 ] && Output[ 0 ]< Position[ 0 ] + Size && Output[ 1 ] > Position[ 1 ] && Output[ 1 ] < Position[ 1 ] + Size ) return true;
	else return false;
}
bool WorldToScreen( Vector from, Vector &to, float *m_vMatrix ) 
{ 
	to.x = m_vMatrix[ 0 ] * from.x + m_vMatrix[ 1 ] * from.y + m_vMatrix[ 2 ] * from.z + m_vMatrix[ 3 ]; 
	to.y = m_vMatrix[ 4 ] * from.x + m_vMatrix[ 5 ] * from.y + m_vMatrix[ 6 ] * from.z + m_vMatrix[ 7 ]; 
	float w = m_vMatrix[ 12 ] * from.x + m_vMatrix[ 13 ] * from.y + m_vMatrix[ 14 ] * from.z + m_vMatrix[ 15 ]; 
	
    if( w < 0.01f ) return false; 

    float invw = 1.0f / w; 
	to.x *= invw; 
	to.y *= invw; 

	float x = ( float )Direct3D9::getWidth() / 2.f; 
    float y = ( float )Direct3D9::getHeight() / 2.f; 
	
    x += 0.5f * to.x * ( float )Direct3D9::getWidth() + 0.5f; 
    y -= 0.5f * to.y * ( float )Direct3D9::getHeight() + 0.5f;

	to.x = x;
	to.y = y;

    return true;
}
void CalculateAngles( Vector vStart, Vector vEnd, Vector& vAngle )
{
	double delta[ 3 ] = { vStart[ 0 ] - vEnd[ 0 ], vStart[ 1 ] - vEnd [ 1 ] , vStart[ 2 ] - vEnd[ 2 ] };
	double hyp = sqrt( delta[ 0 ] * delta[ 0 ] + delta[ 1 ] * delta[ 1 ] );

	vAngle[ 0 ] = ( float ) ( atan( delta[ 2 ] / hyp ) * M_RADPI );
	vAngle[ 1 ] = ( float ) ( atan( delta[ 1 ] / delta[ 0 ] ) * M_RADPI );
	vAngle[ 2 ] = 0.0f;

	if( delta[ 0 ] >= 0.0 ) vAngle[ 1 ] += 180.0f;
}
void MakeVector( Vector pfIn, Vector &pfOut )
{
	float pitch;
	float yaw;
	float tmp;		
	
	pitch = ( float ) ( pfIn[ 0 ] * M_PI / 180);
	yaw = ( float ) ( pfIn[ 1 ] * M_PI / 180);
	tmp = ( float ) cos( pitch );
	
	pfOut[ 0 ] = ( float ) ( -tmp * -cos( yaw ) );
	pfOut[ 1 ] = ( float ) ( sin( yaw )*tmp );
	pfOut[ 2 ] = ( float ) -sin( pitch );
}
void VectorTransform( const Vector in1, const matrix3x4_t& in2, Vector& out )
{
	out[ 0 ] = DotProduct( in1, in2[ 0 ] ) + in2[ 0 ][ 3 ];
	out[ 1 ] = DotProduct( in1, in2[ 1 ] ) + in2[ 1 ][ 3 ];
	out[ 2 ] = DotProduct( in1, in2[ 2 ] ) + in2[ 2 ][ 3 ];
}
void AngleVectors( const Vector angles, Vector &forward, Vector &right, Vector &up )
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[ 0 ] * ( M_PI / 180.f );
	sp = sin( angle );
	cp = cos( angle );

	angle = angles[ 1 ] * ( M_PI / 180.f );
	sy = sin( angle );
	cy = cos( angle );

	if( forward ) 
	{
		forward[ 0 ] = cp * cy;
		forward[ 1 ] = cp * sy;
		forward[ 2 ] = -sp;
	}
	if( right || up ) 
	{
		static float sr, cr;

		angle = angles[ 2 ] * ( M_PI / 180.f );
		sr = sin( angle );
		cr = cos( angle );

		if( right )
		{
			right[ 0 ] = -1 * sr * sp * cy + -1 * cr * -sy;
			right[ 1 ] = -1 * sr * sp * sy + -1 * cr *cy;
			right[ 2 ] = -1 * sr * cp;
		}
		if( up ) 
		{
			up[ 0 ] = cr * sp *cy + -sr * -sy;
			up[ 1 ] = cr * sp *sy + -sr * cy;
			up[ 2 ] = cr * cp;
		}
	}
}
void VectorAngles( const Vector forward, Vector &angles )
{
	float	tmp, yaw, pitch;
	
	if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 )
	{
		yaw = 0;
		if ( forward[ 2 ] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = ( atan2( forward[ 1 ], forward[ 0 ] ) * 180.f / M_PI);
		if ( yaw < 0.f )
			yaw += 360.f;

		tmp = sqrt ( forward[ 0 ] * forward[ 0 ] + forward[ 1 ]*forward[ 1 ] );
		pitch = ( atan2( - forward[ 2 ], tmp ) * 180.f / M_PI );
		if ( pitch < 0.f )
			pitch += 360.f;
	}
	
	angles[ 0 ] = pitch;
	angles[ 1 ] = yaw;
	angles[ 2 ] = 0;
}
float AngleDif( Vector angle, Vector src, Vector dst )
{ 
	Vector ang, aim;
	float angDifference; 

	CalculateAngles( src, dst, ang ); 
	MakeVector( angle, aim ); 
	MakeVector( ang, ang );      

	float mag_s = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) ); 
	float mag_d = sqrt( ( aim[ 0 ] * aim[ 0 ] ) + ( aim[ 1 ] * aim[ 1 ] ) + ( aim[ 2 ] * aim[ 2 ] ) ); 

	float u_dot_v = aim[ 0 ] * ang[ 0 ] + aim[ 1 ] * ang[ 1]  + aim[ 2 ] * ang[ 2 ]; 

	angDifference = acos( u_dot_v / ( mag_s*mag_d ) ) * ( 180.0 / M_PI ); 

	return angDifference; 
}