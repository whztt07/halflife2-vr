#include "cbase.h"
#include "laser_crosshair.h"
#include "tier0/memdbgon.h"

BEGIN_DATADESC( CLaserCrosshair )
	DEFINE_FIELD( m_vecSurfaceNormal,	FIELD_VECTOR ),
	DEFINE_FIELD( m_bVisibleLaserDot,	FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bIsOn,				FIELD_BOOLEAN ),

	//DEFINE_FIELD( m_pNext, FIELD_CLASSPTR ),	// don't save - regenerated by constructor
	DEFINE_THINKFUNC( LaserThink ),
END_DATADESC()



CLaserCrosshair::CLaserCrosshair( void )
{
	m_bIsOn = true;
}

CLaserCrosshair::~CLaserCrosshair( void ) 
{
	
}

void CLaserCrosshair::Precache( void )
{
	PrecacheModel("sprites/laserdot.vmt");
	BaseClass::Precache();
}

CLaserCrosshair *CLaserCrosshair::Create( const Vector &origin, CBaseEntity *pOwner, bool bVisibleDot )
{
	CLaserCrosshair *pLaserDot = (CLaserCrosshair *) CBaseEntity::Create( "env_laserdot", origin, QAngle(0,0,0) );

	if ( pLaserDot == NULL )
		return NULL;

	pLaserDot->m_bVisibleLaserDot = bVisibleDot;
	pLaserDot->SetMoveType( MOVETYPE_NONE );
	pLaserDot->AddSolidFlags( FSOLID_NOT_SOLID );
	pLaserDot->AddEffects( EF_NOSHADOW );
	UTIL_SetSize( pLaserDot, vec3_origin, vec3_origin );

	//Create the graphic
	pLaserDot->SpriteInit( "sprites/laserdot.vmt", origin );

	pLaserDot->SetName( AllocPooledString("TEST") );

	pLaserDot->SetTransparency( kRenderGlow, 255, 255, 255, 255, kRenderFxNoDissipation );
	pLaserDot->SetScale( 0.0625f );

	pLaserDot->SetOwnerEntity( pOwner );

	pLaserDot->SetContextThink( &CLaserCrosshair::LaserThink, gpGlobals->curtime + 0.1f, "LaserCrosshairThinkContext" );
	pLaserDot->SetSimulatedEveryTick( true );

	if ( !bVisibleDot )
	{
		pLaserDot->MakeInvisible();
	}

	return pLaserDot;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLaserCrosshair::LaserThink( void )
{
	SetNextThink( gpGlobals->curtime + 0.05f, "LaserCrosshairThinkContext" );

	if ( GetOwnerEntity() == NULL )
		return;

	Vector	viewDir = GetAbsOrigin() - GetOwnerEntity()->GetAbsOrigin();
	float	dist = VectorNormalize( viewDir );

	float	scale = RemapVal( dist, 32, 1024, 0.01f, 0.5f );
	float	scaleOffs = random->RandomFloat( -scale * 0.25f, scale * 0.25f );

	scale = clamp( scale + scaleOffs, 0.1f, 32.0f );

	SetScale( scale );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLaserCrosshair::TurnOn( void )
{
	m_bIsOn = true;
	if ( m_bVisibleLaserDot )
	{
		BaseClass::TurnOn();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLaserCrosshair::TurnOff( void )
{
	m_bIsOn = false;
	if ( m_bVisibleLaserDot )
	{
		BaseClass::TurnOff();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLaserCrosshair::MakeInvisible( void )
{
	BaseClass::TurnOff();
}


void CLaserCrosshair::SetLaserPosition( const Vector &origin, const Vector &normal )
{
	SetAbsOrigin( origin );
	m_vecSurfaceNormal = normal;
}
