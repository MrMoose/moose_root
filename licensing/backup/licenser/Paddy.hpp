#ifndef DEFINE_LICENSER_PADDY_HPP
#define DEFINE_LICENSER_PADDY_HPP

#include <cstdarg>

namespace Licenser {

class Paddy {

	public:
		template<typename Str>
		static Str &assignf(Str &str, const char *fmt, ...) {
			
			va_list ap;

			va_start(ap, fmt);
			assignvf(str, fmt,ap);
			va_end(ap);
			return str;
		}


		template<typename Str>
		static Str &assignvf(Str &str, const char *fmt, va_list ap) {

			char  buf[4096] = {0};
			char *ch = buf;
			int   num = 0;

			if ((num = vsnprintf(ch, sizeof(buf), fmt, ap)) >= 4096) {
				ch = (char *)calloc(num + 1, 1);
				vsnprintf(ch, sizeof(buf), fmt, ap);
				str.assign(ch, num);
				free(ch);
			} else {
				// don't append nullbyte
				str.assign(ch, num);
			}
			return str;
		}
		
		template<typename Str>
		static Str &appendf(Str &str, const char *fmt, ...) {
			
			va_list ap;

			va_start(ap, fmt);
			appendvf(str, fmt,ap);
			va_end(ap);
			return str;
		}


		template<typename Str>
		static Str &appendvf(Str &str, const char *fmt, va_list ap) {

			char  buf[4096] = {0};
			char *ch = buf;
			int   num = 0;

			if ((num = vsnprintf(ch, sizeof(buf), fmt, ap)) >= 4096) {
				ch = (char *)calloc(num + 1, 1);
				vsnprintf(ch, sizeof(buf), fmt, ap);
				str.append(ch, num);
				free(ch);
			} else {
				// don't append nullbyte
				str.append(ch, num);
			}
			return str;
		}
}; 

}
#endif

