use std::collections::HashMap;

async fn get(target: &'static str) {
    let resp = reqwest::get(target)
        .await
        .unwrap()
        .json::<HashMap<String, String>>()
        .await
        .unwrap();
    println!("{:#?}", resp);
}

async fn do_requests_async(targets: Vec<&'static str>) {
    let mut get_futures = Vec::new();
    for target in targets {
        get_futures.push(get(target));
    }

    for task in get_futures {
        tokio::spawn(task);
    }
 
    loop{}
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    do_requests_async(vec![
        "https://api.binance.com/api/v3/ticker/price?symbol=ETHBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=LTCBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=BNBBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=XLMBTC",
        "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT",
    ]).await;
    Ok(())
}
