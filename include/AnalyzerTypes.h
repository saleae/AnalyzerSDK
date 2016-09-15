#ifndef ANALYZERPUBLICTYPES
#define ANALYZERPUBLICTYPES

#include <LogicPublicTypes.h>

namespace AnalyzerEnums
{
	enum ShiftOrder { MsbFirst, LsbFirst };
	enum EdgeDirection { PosEdge, NegEdge };
	enum Edge { LeadingEdge, TrailingEdge };
	enum Parity { None, Even, Odd };
	enum Acknowledge { Ack, Nak };
	enum Sign { UnsignedInteger, SignedInteger };
};

#endif //ANALYZERPUBLICTYPES
