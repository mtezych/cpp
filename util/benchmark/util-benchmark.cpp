
#include <util/vec.h>

#include <benchmark/benchmark.h>

namespace
{
	void BenchmarkVector (benchmark::State& state)
	{
		auto v = util::vec4 { 0.1f, 0.2f, 0.4f, 1.0f };

		while (state.KeepRunning())
		{
			v[1] = v.z;

			state.PauseTiming();
			{
				benchmark::DoNotOptimize(v[0] = v.y);

				benchmark::ClobberMemory();
			}
			state.ResumeTiming();
		}
	}
}
BENCHMARK(BenchmarkVector);

BENCHMARK_MAIN();
