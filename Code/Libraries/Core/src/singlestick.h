#ifndef SINGLESTICK_H
#define SINGLESTICK_H

// Single-stick control mode (ELDRITCH_SINGLE_STICK=1), for handhelds with one
// analog stick. XInputController feeds the physical stick to the right stick
// (look) and uses this to turn the D-pad into the left stick (move).
// Kept free of engine and SDL dependencies so it can be unit tested.

struct SSingleStickDPad
{
	bool	Up;
	bool	Down;
	bool	Left;
	bool	Right;
	bool	Select;	// While held, D-pad left/right stay D-pad buttons (lean) instead of strafing
};

struct SSingleStickMove
{
	float	X;			// [-1, 1], right is positive
	float	Y;			// [-1, 1], forward (D-pad up) is positive, like the left stick
	bool	ReportLeft;	// Whether D-pad left/right are still reported as buttons
	bool	ReportRight;
};

inline SSingleStickMove ComputeSingleStickMove( const SSingleStickDPad& DPad )
{
	SSingleStickMove Move;

	const bool StrafeLeft	= DPad.Left && !DPad.Select;
	const bool StrafeRight	= DPad.Right && !DPad.Select;

	Move.X = ( StrafeRight ? 1.0f : 0.0f ) - ( StrafeLeft ? 1.0f : 0.0f );
	Move.Y = ( DPad.Up ? 1.0f : 0.0f ) - ( DPad.Down ? 1.0f : 0.0f );

	// Keep diagonals at unit length, like a stick pushed to the edge
	if( Move.X != 0.0f && Move.Y != 0.0f )
	{
		static const float kInvSqrt2 = 0.70710678f;
		Move.X *= kInvSqrt2;
		Move.Y *= kInvSqrt2;
	}

	Move.ReportLeft		= DPad.Left && DPad.Select;
	Move.ReportRight	= DPad.Right && DPad.Select;

	return Move;
}

#endif // SINGLESTICK_H
