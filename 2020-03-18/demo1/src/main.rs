// Taken from: https://rust-lang.github.io/async-book/01_getting_started/04_async_await_primer.html

// The executor I talked about
use futures::executor::block_on;

// An asynchronous function
async fn hello_world() {
    println!("hello, world!");
}

fn main() {
    let future = hello_world(); // Nothing is printed
    block_on(future); // `future` is run and "hello, world!" is printed
}
