
#ifndef PLATFORM_XCB_WINDOW
#define PLATFORM_XCB_WINDOW

#include <xcb/Connection.h>

#include <util/vec.h>

#include <string>

namespace xcb
{
	struct Window
	{
	private:

		struct Event
		{
			xcb_generic_event_t* xcbEvent;

			Event (xcb_generic_event_t* const xcbEvent);
			~Event ();

			Event (Event&& event);
			Event (const Event& event) = delete;

			Event& operator = (Event&& event);
			Event& operator = (const Event& event) = delete;

			explicit operator bool () const;

			template <typename EventType>
			const EventType& CastTo() const
			{
				const auto castedEvent = reinterpret_cast<const EventType*>(xcbEvent);

				assert(castedEvent != nullptr);

				return *castedEvent;
			}
		};

		struct Atom
		{
			xcb_intern_atom_reply_t* xcbAtomReply;

			Atom (const Connection& connection, const std::string& atomName);
			~Atom ();

			Atom (Atom&& atom);
			Atom (const Atom& atom) = delete;

			Atom& operator = (Atom&& atom);
			Atom& operator = (const Atom& atom) = delete;
		};

	public:

		const Connection* connection;
		xcb_window_t      xcbWindow;

		Window (const Connection& connection, const util::uvec2& size);
		~Window ();

		Window (Window&& window);
		Window (const Window& window) = delete;

		Window& operator = (Window&& window);
		Window& operator = (const Window& window) = delete;

		bool HandleEvent (const Event& event) const;

		template <typename Callable>
		void ReceiveMessages(const Callable& render) const
		{
			while (true)
			{
				const auto event = Event
				{
					xcb_poll_for_event(connection->xcbConnection)
				};

				if (event)
				{
					if (!HandleEvent(event))
					{
						break;
					}
				}
				else
				{
					render();
				}
			}
		}
	};
}

#endif
