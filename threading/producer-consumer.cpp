
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace threading
{
	struct Product
	{
		char character;

		Product()
		:
			character { '\0' }
		{
		}

		explicit Product(char character)
		:
			character { character }
		{
			std::this_thread::sleep_for(std::chrono::milliseconds { 100 });
		}
	};

	std::ostream& operator << (std::ostream& ostream, const Product& product)
	{
		return ostream << product.character;
	}

	struct Shelve
	{
		std::queue<Product>     products;
		std::mutex              mutex;
		std::condition_variable condition;
	};

	struct Producer
	{
		static void produce(Shelve& shelve)
		{
			auto product = Product { '#' };
			{
				std::lock_guard<std::mutex> lock { shelve.mutex };

				shelve.products.emplace(std::move(product));
			}
			shelve.condition.notify_one();
		}
	};

	class Consumer
	{
	private:
		Product consumedProduct;

	public:
		void consume(Shelve& shelve)
		{
			{
				std::unique_lock<std::mutex> lock { shelve.mutex };

				const auto canConsume = [&shelve]()
				{
					return !shelve.products.empty();
				};
				shelve.condition.wait(lock, canConsume);

				consumedProduct = std::move(shelve.products.front());

				shelve.products.pop();
			}
			std::cout << consumedProduct << std::endl;
		}
	};
}

int main()
{
	threading::Shelve shelve { };

	auto producerThread = std::thread
	{
		[&shelve] ()
		{
			const auto producer = threading::Producer { };

			while (true)
			{
				producer.produce(shelve);
			}
		}
	};
	auto consumerThread = std::thread
	{
		[&shelve] ()
		{
			auto consumer = threading::Consumer { };

			while (true)
			{
				consumer.consume(shelve);
			}
		}
	};

	producerThread.join();
	consumerThread.join();

	return 0;
}
