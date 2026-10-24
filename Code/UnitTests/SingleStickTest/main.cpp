// Unit test for the single-stick D-pad remap (Libraries/Core/src/singlestick.h).
// Build with -DELD_BUILD_TESTS=ON; returns non-zero on failure.

#include "singlestick.h"

#include <cmath>
#include <cstdio>

static int gFailures = 0;

static void Check( const bool Condition, const char* const Description )
{
	if( !Condition )
	{
		printf( "FAIL: %s\n", Description );
		++gFailures;
	}
}

static bool Near( const float A, const float B )
{
	return std::fabs( A - B ) < 1.0e-5f;
}

static SSingleStickMove Run( const bool Up, const bool Down, const bool Left, const bool Right, const bool Select )
{
	SSingleStickDPad DPad;
	DPad.Up		= Up;
	DPad.Down	= Down;
	DPad.Left	= Left;
	DPad.Right	= Right;
	DPad.Select	= Select;
	return ComputeSingleStickMove( DPad );
}

int main()
{
	SSingleStickMove Move;

	Move = Run( false, false, false, false, false );
	Check( Move.X == 0.0f && Move.Y == 0.0f, "idle D-pad gives no movement" );
	Check( !Move.ReportLeft && !Move.ReportRight, "idle D-pad reports no lean" );

	Move = Run( true, false, false, false, false );
	Check( Move.X == 0.0f && Move.Y == 1.0f, "up moves forward (+Y, same sign as the left stick)" );

	Move = Run( false, true, false, false, false );
	Check( Move.X == 0.0f && Move.Y == -1.0f, "down moves back (-Y)" );

	Move = Run( false, false, true, false, false );
	Check( Move.X == -1.0f && Move.Y == 0.0f, "left strafes left" );
	Check( !Move.ReportLeft, "left without Select does not lean" );

	Move = Run( false, false, false, true, false );
	Check( Move.X == 1.0f && Move.Y == 0.0f, "right strafes right" );
	Check( !Move.ReportRight, "right without Select does not lean" );

	Move = Run( true, false, false, true, false );
	Check( Near( Move.X, 0.70710678f ) && Near( Move.Y, 0.70710678f ), "diagonal is normalized" );
	Check( Near( Move.X * Move.X + Move.Y * Move.Y, 1.0f ), "diagonal has unit length" );

	Move = Run( false, false, true, false, true );
	Check( Move.X == 0.0f && Move.ReportLeft, "Select + left leans and does not strafe" );

	Move = Run( false, false, false, true, true );
	Check( Move.X == 0.0f && Move.ReportRight, "Select + right leans and does not strafe" );

	Move = Run( true, false, true, false, true );
	Check( Move.X == 0.0f && Move.Y == 1.0f && Move.ReportLeft, "Select + up/left moves forward at full speed while leaning" );

	Move = Run( true, true, true, true, false );
	Check( Move.X == 0.0f && Move.Y == 0.0f, "opposite directions cancel" );

	Move = Run( false, false, false, false, true );
	Check( Move.X == 0.0f && Move.Y == 0.0f && !Move.ReportLeft && !Move.ReportRight, "Select alone does nothing" );

	if( gFailures == 0 )
	{
		printf( "SingleStickTest: all checks passed\n" );
	}
	return gFailures == 0 ? 0 : 1;
}
