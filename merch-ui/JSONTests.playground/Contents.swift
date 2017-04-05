//: Playground - noun: a place where people can play

import UIKit

var str = "Hello, playground"

class Item  {
    var id: Int
    init(id: Int) {
        self.id = id
    }
}

class User {
    var pin: Int
    init(pin: Int) {
        self.pin = pin
    }
}

let user = User(pin: 12873192739)
let item = Item(id: 1)

let json: [String: Any] = ["id": [item.id],
 "pin": user.pin]


do {
    let jsonData = try JSONSerialization.data(withJSONObject: json, options: .prettyPrinted)
    // here "jsonData" is the dictionary encoded in JSON data
    
    let decoded = try JSONSerialization.jsonObject(with: jsonData, options: [])
    // here "decoded" is of type `Any`, decoded from JSON data
    
    // you can now cast it with the right type
    if let dictFromJSON = decoded as? [String:String] {
        // use dictFromJSON
    }
} catch {
    print(error.localizedDescription)
}
