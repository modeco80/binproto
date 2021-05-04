#include <binproto/Version.h>
#include <sstream>

namespace {
	template <class T>
	struct CheckedInitalization {
		template <class F>
		explicit CheckedInitalization(const F& fun) {
			if(initalized)
				return;

			fun(var);
			initalized = true;
		}

		T& Get() {
			return var;
		}

	   private:
		T var;
		bool initalized = false;
	};
} // namespace

namespace binproto::version {
	const std::string& String() {
		// Unlock Performace Now:tm:
		static CheckedInitalization<std::string> str_initalizer([](std::string& str) {
			std::stringstream ss;
			ss << (unsigned)version::major << '.' << (unsigned)version::minor;
			str = ss.str();
		});

		return str_initalizer.Get();
	}
} // namespace binproto::version