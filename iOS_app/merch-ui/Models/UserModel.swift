//
//  UserModel.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

class UserModel {
    static var shared: UserModel?

    private init() { }
    
    var netID = ""
    var token = ""
    
    func reset() {
        netID = ""
        token = ""
    }
    
    static func load(json: [String: Any]) {
        
        
        // TODO: - fill me
    }
}
