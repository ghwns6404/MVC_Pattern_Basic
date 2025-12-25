// FakestaException.cs

using System;

namespace _0513_2_Fakesta.Error
{
    internal class FakestaException : Exception
    {
        public FakestaException() { }

        public FakestaException(string message)
            : base(message) { }

        public FakestaException(string message, Exception inner)
            : base(message, inner) { }
    }

    internal class FakestaMemberNotFoundException : FakestaException
    {
        public FakestaMemberNotFoundException() { }

        public FakestaMemberNotFoundException(string message)
            : base(message) { }

        public FakestaMemberNotFoundException(string message, Exception inner)
            : base(message, inner) { }
    }

    internal class FakestaSessionNotFoundException : FakestaException
    {
        public FakestaSessionNotFoundException() { }

        public FakestaSessionNotFoundException(string message)
            : base(message) { }

        public FakestaSessionNotFoundException(string message, Exception inner)
            : base(message, inner) { }
    }

    internal class FakestaPostNotFoundException : FakestaException
    {
        public FakestaPostNotFoundException() { }

        public FakestaPostNotFoundException(string message)
            : base(message) { }

        public FakestaPostNotFoundException(string message, Exception inner)
            : base(message, inner) { }
    }

    internal class FakestaPostFileNotFoundException : FakestaException
    {
        public FakestaPostFileNotFoundException() { }

        public FakestaPostFileNotFoundException(string message)
            : base(message) { }

        public FakestaPostFileNotFoundException(string message, Exception inner)
            : base(message, inner) { }
    }

    internal class FakestaAuthException : FakestaException
    {
        public FakestaAuthException() { }

        public FakestaAuthException(string message)
            : base(message) { }

        public FakestaAuthException(string message, Exception inner)
            : base(message, inner) { }
    }
}
