// 🍤🍤🍤

#include "c74_min.h"
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <boost/filesystem.hpp>

namespace py = pybind11;

class python : public c74::min::object<python> {

public:
    MIN_DESCRIPTION{"Embedded python interpreter"};
    MIN_TAGS{"utilities"};
    MIN_AUTHOR{"Hiroki Sakuma"};
    MIN_RELATED{"js"};

    python(const std::vector<c74::min::atom>& atoms = {}) {
        if (atoms.size() == 1)
        {
            const auto current_dirname = "/Users/hirokisakuma/Documents/Max 8/Packages/min-devkit/source/projects/min.python";
            py::module::import("sys").attr("path").cast<py::list>().append(current_dirname);

            auto module_name = static_cast<std::string>(atoms.front());
            m_function = py::module::import(module_name.c_str()).attr(module_name.c_str());

            auto signature = py::module::import("signature").attr("signature");
            std::tie(m_input_annotations, m_output_annotations) = signature(module_name, module_name).cast<std::tuple<std::vector<std::string>, std::vector<std::string>>>();
            
            for (const auto& input_annotation : m_input_annotations) {
                m_inlets.emplace_back(std::make_unique<c74::min::inlet<>>(this, "(" + input_annotation + ")", input_annotation));
                m_inputs.append(0);
            }
            for (const auto& output_annotation : m_output_annotations) {
                m_outlets.emplace_back(std::make_unique<c74::min::outlet<>>(this, "(" + output_annotation + ")", output_annotation));
            }
        }
        else {
            c74::min::error("only a script name required");
        }
    }

    c74::min::message<> number {this, "number", 
        [this](const std::vector<c74::min::atom>& atoms, const int inlet) -> std::vector<c74::min::atom> {
            assert(atoms.size() == 1);
            if (m_input_annotations[inlet] == "int") {
                m_inputs[inlet] = static_cast<int>(atoms.front());
            } else if (m_input_annotations[inlet] == "float") {
                m_inputs[inlet] = static_cast<float>(atoms.front());
            } else {
                c74::min::error("invalid input annotation");
            }
            m_outputs = m_function(*m_inputs).cast<py::tuple>();
            for (auto outlet = 0; outlet < m_outlets.size(); ++outlet) {
                if (m_output_annotations[outlet] == "int") {
                    m_outlets[outlet]->send(m_outputs[outlet].cast<int>());
                } else if (m_output_annotations[outlet] == "float") {
                    m_outlets[outlet]->send(m_outputs[outlet].cast<float>());
                } else {
                    c74::min::error("invalid output annotation");
                }
            }
            return {};
        }
    };
protected:

    std::vector<std::unique_ptr<c74::min::inlet<>>> m_inlets;
    std::vector<std::unique_ptr<c74::min::outlet<>>> m_outlets;

    std::vector<std::string> m_input_annotations;
    std::vector<std::string> m_output_annotations;

    py::scoped_interpreter m_interpreter;
    py::object m_function;

    py::list m_inputs;
    py::tuple m_outputs;
};

MIN_EXTERNAL(python);
