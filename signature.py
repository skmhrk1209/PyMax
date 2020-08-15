import inspect
import importlib
import typing


def signature(module_name, function_name):
    module = importlib.import_module(module_name)
    function = getattr(module, function_name)
    function_signature = inspect.signature(function)
    assert all(parameter.annotation is not parameter.empty for parameter in function_signature.parameters.values())
    assert function_signature.return_annotation is not function_signature.empty
    input_annotations = [str(parameter.annotation).split("'")[1] for parameter in function_signature.parameters.values()]
    output_annotations = [str(annotation).split("'")[1] for annotation in function_signature.return_annotation.__args__]
    return input_annotations, output_annotations
