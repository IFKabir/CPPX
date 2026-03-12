

#include "../include/cppx.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class Benchmark
{
  public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double, std::milli>;

    void start()
    {
        m_start = Clock::now();
    }

    void stop()
    {
        m_end = Clock::now();
        m_elapsed = std::chrono::duration_cast<Duration>(m_end - m_start);
    }

    double elapsed_ms() const
    {
        return m_elapsed.count();
    }

  private:
    TimePoint m_start{};
    TimePoint m_end{};
    Duration m_elapsed{};
};

static std::vector<int> generate_unique_random(std::size_t count, unsigned seed = 42)
{
    std::mt19937 rng(seed);
    std::set<int> unique_set;
    while (unique_set.size() < count)
        unique_set.insert(static_cast<int>(rng()));
    return {unique_set.begin(), unique_set.end()};
}

static std::vector<int> pick_random(const std::vector<int> &source, std::size_t count, unsigned seed = 123)
{
    std::mt19937 rng(seed);
    std::vector<int> copy = source;
    std::shuffle(copy.begin(), copy.end(), rng);
    copy.resize(std::min(count, copy.size()));
    return copy;
}

struct BenchmarkResult
{
    std::string structure;
    std::size_t n;
    double insert_ms;
    double lookup_ms;
    double delete_ms;
};

static BenchmarkResult bench_std_set(std::size_t n, const std::vector<int> &data, const std::vector<int> &lookup_data,
                                     const std::vector<int> &delete_data)
{
    Benchmark bm;
    std::set<int> s;

    bm.start();
    for (int v : data)
        s.insert(v);
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = (s.find(v) != s.end());
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        s.erase(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"std::set", n, ins, lkp, del};
}

static BenchmarkResult bench_std_map(std::size_t n, const std::vector<int> &data, const std::vector<int> &lookup_data,
                                     const std::vector<int> &delete_data)
{
    Benchmark bm;
    std::map<int, int> m;

    bm.start();
    for (int v : data)
        m[v] = v;
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = (m.find(v) != m.end());
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        m.erase(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"std::map", n, ins, lkp, del};
}

static BenchmarkResult bench_std_unordered_set(std::size_t n, const std::vector<int> &data,
                                               const std::vector<int> &lookup_data, const std::vector<int> &delete_data)
{
    Benchmark bm;
    std::unordered_set<int> s;
    s.reserve(n);

    bm.start();
    for (int v : data)
        s.insert(v);
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = (s.find(v) != s.end());
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        s.erase(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"std::unordered_set", n, ins, lkp, del};
}

static BenchmarkResult bench_avl(std::size_t n, const std::vector<int> &data, const std::vector<int> &lookup_data,
                                 const std::vector<int> &delete_data)
{
    Benchmark bm;
    stl_ext::AVLTree<int> tree;

    bm.start();
    for (int v : data)
        tree.insert(v);
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = tree.contains(v);
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        tree.remove(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"stl_ext::AVLTree", n, ins, lkp, del};
}

static BenchmarkResult bench_rbt(std::size_t n, const std::vector<int> &data, const std::vector<int> &lookup_data,
                                 const std::vector<int> &delete_data)
{
    Benchmark bm;
    stl_ext::RBTree<int> tree;

    bm.start();
    for (int v : data)
        tree.insert(v);
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = tree.contains(v);
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        tree.remove(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"stl_ext::RBTree", n, ins, lkp, del};
}

static BenchmarkResult bench_bst(std::size_t n, const std::vector<int> &data, const std::vector<int> &lookup_data,
                                 const std::vector<int> &delete_data)
{
    Benchmark bm;
    stl_ext::BST<int> tree;

    bm.start();
    for (int v : data)
        tree.insert(v);
    bm.stop();
    double ins = bm.elapsed_ms();

    bm.start();
    volatile bool sink = false;
    for (int v : lookup_data)
        sink = tree.contains(v);
    bm.stop();
    double lkp = bm.elapsed_ms();
    (void)sink;

    bm.start();
    for (int v : delete_data)
        tree.remove(v);
    bm.stop();
    double del = bm.elapsed_ms();

    return {"stl_ext::BST", n, ins, lkp, del};
}

static std::string format_n(std::size_t n)
{
    if (n >= 1'000'000)
        return std::to_string(n / 1'000'000) + "M";
    if (n >= 1'000)
        return std::to_string(n / 1'000) + "K";
    return std::to_string(n);
}

static void print_separator(int w)
{
    std::cout << std::string(static_cast<std::size_t>(w), '-') << "\n";
}

static void print_table(const std::vector<BenchmarkResult> &results)
{
    const int w_s = 22, w_n = 8, w_t = 16;
    const int w_total = w_s + w_n + w_t * 3 + 6;

    std::cout << "\n";
    print_separator(w_total);
    std::cout << std::left << std::setw(w_s) << "Structure" << std::right << std::setw(w_n) << "N" << std::setw(w_t)
              << "Insert (ms)" << std::setw(w_t) << "Lookup (ms)" << std::setw(w_t) << "Delete (ms)" << "\n";
    print_separator(w_total);

    for (const auto &r : results)
    {
        std::cout << std::left << std::setw(w_s) << r.structure << std::right << std::setw(w_n) << format_n(r.n)
                  << std::setw(w_t) << std::fixed << std::setprecision(2) << r.insert_ms << std::setw(w_t)
                  << r.lookup_ms << std::setw(w_t) << r.delete_ms << "\n";
    }
    print_separator(w_total);
    std::cout << "\n";
}

static void export_csv(const std::vector<BenchmarkResult> &results, const std::string &filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        std::cerr << "[error] Cannot open " << filename << " for writing.\n";
        return;
    }
    out << "structure,n,insert_ms,lookup_ms,delete_ms\n";
    for (const auto &r : results)
    {
        out << r.structure << "," << r.n << "," << std::fixed << std::setprecision(4) << r.insert_ms << ","
            << r.lookup_ms << "," << r.delete_ms << "\n";
    }
    std::cout << "[info] CSV exported to " << filename << "\n";
}

static std::string svg_color(const std::string &structure)
{
    if (structure == "std::set")
        return "#6366f1";
    if (structure == "std::map")
        return "#8b5cf6";
    if (structure == "std::unordered_set")
        return "#a78bfa";
    if (structure == "stl_ext::AVLTree")
        return "#06b6d4";
    if (structure == "stl_ext::RBTree")
        return "#10b981";
    return "#f59e0b";
}

static std::string svg_escape(const std::string &s)
{
    std::string out;
    for (char c : s)
    {
        switch (c)
        {
        case '&':
            out += "&amp;";
            break;
        case '<':
            out += "&lt;";
            break;
        case '>':
            out += "&gt;";
            break;
        case '"':
            out += "&quot;";
            break;
        default:
            out += c;
        }
    }
    return out;
}

static std::string to_str(double v, int prec = 1)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << v;
    return ss.str();
}

static void emit_subchart(std::ostream &svg, const std::string &title, const std::vector<BenchmarkResult> &results,
                          const std::vector<std::string> &structures, const std::vector<std::size_t> &sizes,
                          double (*value_fn)(const BenchmarkResult &), double ox, double oy, double chart_w,
                          double chart_h)
{

    double max_val = 0;
    for (auto &r : results)
        max_val = std::max(max_val, value_fn(r));
    if (max_val < 1e-9)
        max_val = 1;
    max_val *= 1.15;

    const double margin_left = 60;
    const double margin_bottom = 40;
    const double margin_top = 35;
    const double plot_w = chart_w - margin_left - 10;
    const double plot_h = chart_h - margin_bottom - margin_top;

    double px = ox + margin_left;
    double py = oy + margin_top;

    svg << "<rect x=\"" << ox << "\" y=\"" << oy << "\" width=\"" << chart_w << "\" height=\"" << chart_h
        << "\" rx=\"8\" fill=\"#1e293b\"/>\n";

    svg << "<text x=\"" << ox + chart_w / 2 << "\" y=\"" << oy + 22
        << "\" text-anchor=\"middle\" font-size=\"14\" font-weight=\"bold\" fill=\"white\">" << title << "</text>\n";

    for (int i = 0; i <= 4; ++i)
    {
        double frac = i / 4.0;
        double yy = py + plot_h - frac * plot_h;
        double label_val = frac * max_val;
        svg << "<line x1=\"" << px << "\" y1=\"" << yy << "\" x2=\"" << px + plot_w << "\" y2=\"" << yy
            << "\" stroke=\"#334155\" stroke-width=\"0.5\"/>\n";
        svg << "<text x=\"" << px - 6 << "\" y=\"" << yy + 4
            << "\" text-anchor=\"end\" font-size=\"9\" fill=\"#94a3b8\">" << to_str(label_val, (max_val > 100 ? 0 : 1))
            << "</text>\n";
    }

    svg << "<text x=\"" << ox + 14 << "\" y=\"" << py + plot_h / 2
        << "\" text-anchor=\"middle\" font-size=\"10\" fill=\"#94a3b8\" " << "transform=\"rotate(-90," << ox + 14 << ","
        << py + plot_h / 2 << ")\">Time (ms)</text>\n";

    std::size_t n_groups = sizes.size();
    std::size_t n_bars = structures.size();
    double group_w = plot_w / static_cast<double>(n_groups);
    double bar_w = group_w / (static_cast<double>(n_bars) + 1.5);
    double gap = bar_w * 0.25;

    for (std::size_t gi = 0; gi < n_groups; ++gi)
    {
        double gx = px + gi * group_w;

        svg << "<text x=\"" << gx + group_w / 2 << "\" y=\"" << py + plot_h + 18
            << "\" text-anchor=\"middle\" font-size=\"11\" fill=\"#94a3b8\">" << format_n(sizes[gi]) << "</text>\n";

        std::size_t bar_idx = 0;
        for (std::size_t si = 0; si < n_bars; ++si)
        {

            const BenchmarkResult *match = nullptr;
            for (auto &r : results)
            {
                if (r.structure == structures[si] && r.n == sizes[gi])
                {
                    match = &r;
                    break;
                }
            }
            if (!match)
                continue;

            double val = value_fn(*match);
            double bh = (val / max_val) * plot_h;
            double bx = gx + gap + bar_idx * (bar_w + gap);
            double by = py + plot_h - bh;

            svg << "<rect x=\"" << bx << "\" y=\"" << by << "\" width=\"" << bar_w << "\" height=\"" << bh
                << "\" rx=\"2\" fill=\"" << svg_color(structures[si]) << "\" opacity=\"0.9\"/>\n";

            svg << "<text x=\"" << bx + bar_w / 2 << "\" y=\"" << by - 3
                << "\" text-anchor=\"middle\" font-size=\"8\" font-weight=\"bold\" fill=\"white\">" << to_str(val)
                << "</text>\n";

            ++bar_idx;
        }
    }
}

static void generate_svg(const std::vector<BenchmarkResult> &results, const std::string &filename)
{

    std::vector<std::string> structures;
    std::vector<std::size_t> sizes;
    for (auto &r : results)
    {
        if (std::find(structures.begin(), structures.end(), r.structure) == structures.end())
            structures.push_back(r.structure);
        if (std::find(sizes.begin(), sizes.end(), r.n) == sizes.end())
            sizes.push_back(r.n);
    }
    std::sort(sizes.begin(), sizes.end());

    const double chart_w = 380;
    const double chart_h = 300;
    const double pad = 20;
    const double legend_h = 60;
    const double title_h = 40;
    const double total_w = 3 * chart_w + 4 * pad;
    const double total_h = title_h + chart_h + legend_h + 2 * pad;

    std::ofstream f(filename);
    if (!f.is_open())
    {
        std::cerr << "[error] Cannot write " << filename << "\n";
        return;
    }

    f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    f << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 " << total_w << " " << total_h << "\">\n";
    f << "<style>text { font-family: 'Segoe UI', Roboto, Arial, sans-serif; }</style>\n";

    // ── Full background ──
    f << "<rect width=\"100%\" height=\"100%\" rx=\"12\" fill=\"#0f172a\"/>\n";

    // ── Main title ──
    f << "<text x=\"" << total_w / 2 << "\" y=\"28\" text-anchor=\"middle\" font-size=\"20\" "
      << "font-weight=\"bold\" fill=\"white\">CPPX Benchmark Results</text>\n";

    // ── Three sub-charts ──
    auto insert_fn = [](const BenchmarkResult &r) -> double { return r.insert_ms; };
    auto lookup_fn = [](const BenchmarkResult &r) -> double { return r.lookup_ms; };
    auto delete_fn = [](const BenchmarkResult &r) -> double { return r.delete_ms; };

    double y_charts = title_h;
    emit_subchart(f, "Insert Time", results, structures, sizes, insert_fn, pad, y_charts, chart_w, chart_h);
    emit_subchart(f, "Lookup Time", results, structures, sizes, lookup_fn, pad + chart_w + pad, y_charts, chart_w,
                  chart_h);
    emit_subchart(f, "Delete Time", results, structures, sizes, delete_fn, pad + 2 * (chart_w + pad), y_charts, chart_w,
                  chart_h);

    // ── Legend ──
    double ly = y_charts + chart_h + 18;
    double item_w = 170;
    double lx_start = total_w / 2 - (structures.size() * item_w) / 2.0;
    for (std::size_t i = 0; i < structures.size(); ++i)
    {
        double lx = lx_start + i * item_w;
        f << "<rect x=\"" << lx << "\" y=\"" << ly << "\" width=\"14\" height=\"14\" rx=\"3\" fill=\""
          << svg_color(structures[i]) << "\"/>\n";
        f << "<text x=\"" << lx + 20 << "\" y=\"" << ly + 12 << "\" font-size=\"12\" fill=\"white\">"
          << svg_escape(structures[i]) << "</text>\n";
    }

    f << "</svg>\n";
    std::cout << "[info] SVG chart saved to " << filename << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[])
{
    // Accept optional output directory (set by CMake)
    std::string out_dir = ".";
    if (argc > 1)
        out_dir = argv[1];

    const std::vector<std::size_t> sizes = {10'000, 100'000, 1'000'000};

    std::vector<BenchmarkResult> all_results;
    all_results.reserve(sizes.size() * 6);

    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            CPPX  Benchmark  Suite  v2.0                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    for (std::size_t n : sizes)
    {
        std::cout << "\n▶ Benchmarking N = " << format_n(n) << " (" << n << " elements) ...\n";

        auto data = generate_unique_random(n);
        auto lookup_data = pick_random(data, n / 2);
        auto delete_data = pick_random(data, n / 10);

        std::mt19937 rng(99);
        std::shuffle(data.begin(), data.end(), rng);

        std::cout << "  ├─ std::set            ... " << std::flush;
        auto r1 = bench_std_set(n, data, lookup_data, delete_data);
        std::cout << "done\n";

        std::cout << "  ├─ std::map            ... " << std::flush;
        auto r_map = bench_std_map(n, data, lookup_data, delete_data);
        std::cout << "done\n";

        std::cout << "  ├─ std::unordered_set  ... " << std::flush;
        auto r_uset = bench_std_unordered_set(n, data, lookup_data, delete_data);
        std::cout << "done\n";

        std::cout << "  ├─ stl_ext::AVLTree    ... " << std::flush;
        auto r2 = bench_avl(n, data, lookup_data, delete_data);
        std::cout << "done\n";

        std::cout << "  ├─ stl_ext::RBTree     ... " << std::flush;
        auto r_rbt = bench_rbt(n, data, lookup_data, delete_data);
        std::cout << "done\n";

        if (n <= 100'000)
        {
            std::cout << "  └─ stl_ext::BST        ... " << std::flush;
            auto r3 = bench_bst(n, data, lookup_data, delete_data);
            std::cout << "done\n";
            all_results.push_back(r3);
        }
        else
        {
            std::cout << "  └─ stl_ext::BST        ... skipped (N too large for unbalanced tree)\n";
        }

        all_results.push_back(r1);
        all_results.push_back(r_map);
        all_results.push_back(r_uset);
        all_results.push_back(r2);
        all_results.push_back(r_rbt);
    }

    // Sort: group by N, then by structure name
    std::sort(all_results.begin(), all_results.end(), [](const BenchmarkResult &a, const BenchmarkResult &b) {
        if (a.n != b.n)
            return a.n < b.n;
        return a.structure < b.structure;
    });

    print_table(all_results);
    export_csv(all_results, out_dir + "/docs/benchmark_results.csv");
    generate_svg(all_results, out_dir + "/docs/benchmark_chart.svg");

    return 0;
}
