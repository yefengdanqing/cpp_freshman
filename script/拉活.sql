-- Note: Unless you save your query, these tabs will NOT persist if you clear your cookies or change browsers.

SELECT
    campaign_id,
    COUNT(DISTINCT creative_id) AS creative_count,
    SUM(request) AS total_request,
    SUM(bid) AS total_bid,
    SUM(win) AS total_win,
    SUM(imp) AS total_impressions,
    SUM(click) AS total_clicks,
    SUM(`install`) AS total_installs,
    SUM(spend) AS total_spend,
    SUM(revenue) AS total_revenue,
    SUM(aemedia_revenue) AS total_aemedia_revenue,
    SUM(imp_revenue) AS total_imp_revenue,
    SUM(ae_uv_cnt) AS total_ae_uv,
    SUM(ae_mbr_cnt) AS total_ae_mbr,
    SUM(ae_total_income) AS total_ae_income,
    SUM(content_view_cnt) AS total_content_views,
    SUM(add_cart_cnt) AS total_add_cart,
    SUM(ae_order_cnt) AS total_ae_orders,
    SUM(conversion) AS total_conversions,
    SUM(cps_order_cnt) AS total_cps_orders,
    SUM(order_cnt) AS total_orders,
    SUM(order_cnt_by_click) AS total_orders_by_click,
    SUM(install_by_click) AS total_installs_by_click,
    SUM(session_mbruv1h) AS total_session_mbruv1h,
    SUM(session_paybyrcnt1h) AS total_session_payments,
    SUM(session_paygmv1h) AS total_session_gmv,
    
    -- 计算关键指标
    SUM(win) * 100.0 / NULLIF(SUM(bid), 0) AS win_rate,
    SUM(click) * 100.0 / NULLIF(SUM(imp), 0) AS ctr,
    SUM(`install`) * 100.0 / NULLIF(SUM(click), 0) AS cvr,
    SUM(spend) / NULLIF(SUM(click), 0) AS cpc,
    SUM(spend) / NULLIF(SUM(`install`), 0) AS cpi,
    SUM(revenue) / NULLIF(SUM(spend), 0) AS roi,
    (SUM(bid) * SUM(click) * SUM(`install`) * 1000) / NULLIF(SUM(imp), 0) AS ecpm
    
FROM 
    dsp.dsp_report
WHERE 
    date = '2025-08-08' 
    AND dc_name IN ('aliyun-sg-k8s') 
    AND ad_exchange = '20020' 
    AND model_name IN ('ST', 'UN')
GROUP BY 
    campaign_id
ORDER BY 
    ecpm DESC;  -- 按eCPM降序排列