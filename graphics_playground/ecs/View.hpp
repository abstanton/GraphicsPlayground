#pragma once
#include "ComponentArray.hpp"
#include "Constants.hpp"

namespace ecs {

template <typename... Components>
class View {
 public:
  View(ComponentArray<Components>&... arrays)
      : component_array_tuple_(arrays...), base_comp_array_(nullptr) {
    std::apply(
        [&](auto&... y) {
          std::apply(
              [&](auto&... x) {
                ((x.size() <= y.size() ? base_comp_array_ = &x
                                       : base_comp_array_ = base_comp_array_),
                 ...);
              },
              component_array_tuple_);
        },
        component_array_tuple_);
  }

  template <typename Func>
  void each(Func func) {
    pick_and_each(std::move(func), std::index_sequence_for<Components...>{});
  }

 private:
  template <std::size_t Curr, std::size_t Other, typename... Args>
  [[nodiscard]] auto dispatch_get(
      const std::tuple<Entity, Args...>& curr) const {
    if constexpr (Curr == Other) {
      return std::forward_as_tuple(std::get<Args>(curr)...);
    } else {
      return std::get<Other>(component_array_tuple_)
          .getDataAsTuple(std::get<0>(curr));
    }
  }

  template <std::size_t Curr, typename Func, std::size_t... Index>
  void each(Func func, std::index_sequence<Index...>) const {
    for (const auto curr : std::get<Curr>(component_array_tuple_).each()) {
      const auto entt = std::get<0>(curr);

      if (((Curr == Index ||
            std::get<Index>(component_array_tuple_).contains(entt)) &&
           ...)) {
        std::apply(func, std::tuple_cat(std::make_tuple(entt),
                                        dispatch_get<Curr, Index>(curr)...));
      }
    }
  }

  template <typename Func, std::size_t... Index>
  void pick_and_each(Func func, std::index_sequence<Index...> seq) const {
    ((&std::get<Index>(component_array_tuple_) == base_comp_array_
          ? each<Index>(std::move(func), seq)
          : void()),
     ...);
  }

  void* base_comp_array_;
  std::tuple<ComponentArray<Components>&...> component_array_tuple_;
};
}  // namespace ecs