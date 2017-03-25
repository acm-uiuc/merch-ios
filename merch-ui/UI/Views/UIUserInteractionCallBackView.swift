//
//  UIUserInteractionCallBackView.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/20/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class UIUserInteractionCallBackView: UIView {
    var callback: ((Void) -> Void)?
    
    override func hitTest(_ point: CGPoint, with event: UIEvent?) -> UIView? {
        callback?()
        return super.hitTest(point, with: event)
    }
}
