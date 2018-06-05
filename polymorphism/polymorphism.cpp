
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <string>

//
// NDC London 2017: Sean Parent "Better Code: Runtime Polymorphism"
//
//   ~ https://www.youtube.com/watch?v=QGcVXgEVMJg
//

namespace poly
{
	class Object
	{
	private:

		struct Concept
		{
			virtual ~Concept () = default;

			virtual std::unique_ptr<Concept> Clone () const = 0;

			virtual void Print (std::ostream& ostream) const = 0;
		};

		template <typename Type>
		struct Model final : Concept
		{
			Type value;

			Model (Type value)
			:
				value (std::move(value))
			{
			}

			std::unique_ptr<Concept> Clone () const override
			{
				return std::make_unique<Model>(*this);
			}

			void Print (std::ostream& ostream) const override
			{
				ostream << value;
			}
		};

		std::unique_ptr<Concept> self;

	public:

		template <typename Type>
		Object (Type value)
		:
			self { std::make_unique<Model<Type>>(std::move(value)) }
		{
		}

		Object (Object&& object) noexcept = default;

		Object (const Object& object)
		:
			self { object.self->Clone() }
		{
		}

		Object& operator = (Object&& object) noexcept = default;

		Object& operator = (const Object& object)
		{
			*this = Object { object };

			return *this;
		}

		void Print (std::ostream& ostream) const
		{
			self->Print(ostream);
		}
	};
}

namespace foo
{
	struct Foo
	{
		char  c;
		float f;
		int   i;
	};

	std::ostream& operator << (std::ostream& ostream, const Foo& foo)
	{
		return ostream << "{ " << foo.c << " " << foo.f << " " << foo.i << " }";
	}
}

std::ostream&
operator << (std::ostream& ostream, const std::vector<poly::Object>& objects)
{
	ostream << "[ ";

	for (const auto& object : objects)
	{
		object.Print(ostream);

		ostream << ' ';
	}

	ostream << ']';

	return ostream;
}

int main ()
{
	auto objects = std::vector<poly::Object>
	{
		0, 1.1, 2u, '&',
		std::string { "text" },
		foo::Foo { '#', 2.7f, 54 },
	};

	objects.push_back(objects);
	objects.push_back("~~~~~");

	std::cout << objects << std::endl;

	return 0;
}
