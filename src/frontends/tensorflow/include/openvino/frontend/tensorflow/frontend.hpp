// Copyright (C) 2018-2025 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <functional>
#include <map>

#include "openvino/core/any.hpp"
#include "openvino/core/node_vector.hpp"
#include "openvino/frontend/extension/conversion.hpp"
#include "openvino/frontend/extension/decoder_transformation.hpp"
#include "openvino/frontend/extension/telemetry.hpp"
#include "openvino/frontend/frontend.hpp"
#include "openvino/frontend/input_model.hpp"
#include "openvino/frontend/tensorflow/node_context.hpp"
#include "openvino/frontend/tensorflow/visibility.hpp"
#include "openvino/frontend/visibility.hpp"

namespace ov {
namespace frontend {
namespace tensorflow {
class TENSORFLOW_FRONTEND_API FrontEnd : public ov::frontend::FrontEnd {
public:
    using Ptr = std::shared_ptr<FrontEnd>;
    FrontEnd();

    /// \brief Completely convert the model
    /// \return fully converted ov Model
    std::shared_ptr<Model> convert(const ov::frontend::InputModel::Ptr& model) const override;

    /// \brief Completely convert the remaining, not converted part of a function.
    /// \param partiallyConverted partially converted ov Model
    void convert(const std::shared_ptr<Model>& partiallyConverted) const override;

    /// \brief Convert only those parts of the model that can be converted leaving others
    /// as-is. Converted parts are not normalized by additional transformations; normalize
    /// function or another form of convert function should be called to finalize the
    /// conversion process.
    /// \param model Input model
    /// \return partially converted ov Model
    std::shared_ptr<Model> convert_partially(const ov::frontend::InputModel::Ptr& model) const override;

    /// \brief Convert operations with one-to-one mapping with decoding nodes.
    /// Each decoding node is an ov node representing a single FW operation node with
    /// all attributes represented in FW-independent way.
    /// \param model Input model
    /// \return ov Model after decoding
    std::shared_ptr<Model> decode(const ov::frontend::InputModel::Ptr& model) const override;

    /// \brief Runs normalization passes on function that was loaded with partial conversion
    /// \param Model partially converted ov Model
    void normalize(const std::shared_ptr<ov::Model>& function) const override;

    /// \brief Gets name of this FrontEnd. Can be used by clients
    std::string get_name() const override {
        return "tf";
    }

    void add_extension(const std::shared_ptr<ov::Extension>& extension) override;

protected:
    /// \brief Check if FrontEndTensorflow can recognize model from given parts
    bool supported_impl(const std::vector<ov::Any>& variants) const override;

    ov::frontend::InputModel::Ptr load_impl(const std::vector<ov::Any>& variants) const override;

    TelemetryExtension::Ptr m_telemetry;
    std::vector<DecoderTransformationExtension::Ptr> m_transformation_extensions;
    std::vector<ConversionExtensionBase::Ptr> m_conversion_extensions;

    TranslatorDictionaryType m_op_translators;
};

}  // namespace tensorflow
}  // namespace frontend
}  // namespace ov
