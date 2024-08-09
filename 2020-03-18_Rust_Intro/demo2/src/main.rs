use std::collections::HashMap;

fn do_requests_blocking(targets: Vec<&str>) {
    for target in targets {
        let resp = reqwest::blocking::get(target)
            .unwrap()
            .json::<HashMap<String, String>>()
            .unwrap();
        println!("{:#?}", resp);
    }
}

fn main() {
    do_requests_blocking(vec![
        "https://api.binance.com/api/v3/ticker/price?symbol=ETHBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=LTCBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=BNBBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=XLMBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT",
    ]);
}
