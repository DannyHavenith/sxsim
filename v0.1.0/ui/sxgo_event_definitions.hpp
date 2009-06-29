#if !defined (SXGO_EVENT_DEFINITIONS_HPP)
#define SXGO_EVENT_DEFINITIONS_HPP

struct sxgo_event_definitions
{
	enum
	{
		ID_CreateTree = wxID_HIGHEST+1,
		ID_CreatePerspective,
		ID_CopyPerspectiveCode,
		ID_SingleStep,
		ID_Run,
		ID_Stop,
		ID_Pause,
		ID_FirstPerspective = ID_CreateTree+1000
	};
};

#endif //SXGO_EVENT_DEFINITIONS_HPP
