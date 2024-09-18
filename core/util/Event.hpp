#pragma once

#include <functional>
#include <vector>

namespace Events
{
  template <class Msg>
  class Event
  {
  public:
    using callback = std::function<void(const Msg &)>;

    struct Token
    {
      bool isValid{false};
      std::size_t m_value;

      operator bool() const { return isValid; }
    };

    static const Token Listen(callback cb)
    {
      mRecievers.push_back(cb);
      return Token{true, mRecievers.size() - 1};
    }

    static void Fire(Msg msg)
    {
      for (auto &i : mRecievers)
      {
        std::invoke(i, msg);
      }
    }

    // todo: change void to some form of task
    static void FireAsync(Msg msg)
    {
      // Do nothing for now
    }

    static void Remove(const Token tk)
    {
      mRecievers.erase(mRecievers.begin() + tk.m_value);
    }

  private:
    static std::vector<callback> mRecievers;
  };

  template <class T>
  std::vector<typename Event<T>::callback> Event<T>::mRecievers;
} // namespace Events